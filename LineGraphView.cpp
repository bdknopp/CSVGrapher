/*
 * LineGraphView.cpp: See "LineGraphView.h" for documentation.
 */

#include "LineGraphView.h"

/*
 * Constructor: LineGraphView
 */
LineGraphView::LineGraphView(QWidget *parent) :
  QAbstractItemView(parent),
  scene(new QGraphicsScene())
{
}

/*
 * Destructor: ~LineGraphView
 */
LineGraphView::~LineGraphView()
{
  delete scene;
}

/*
 * Method: setGraphicsView
 */
void LineGraphView::setGraphicsView(QGraphicsView *graphicsView)
{
  sceneRectangle = QRectF(0, 0, 1, 1);
  view = graphicsView;
  view->scale(1,-1);
  view->installEventFilter(this);
}

/*
 * Method: setLabels
 */
void LineGraphView::setLabels(QLabel *xLabel, QLabel *yLabel)
{
  this->xLabel = xLabel;
  this->yLabel = yLabel;
}

/*
 * Method: visualRect
 */
QRect LineGraphView::visualRect(const QModelIndex &index) const
{
  if (!index.isValid())
    return QRect();
  else
    // DETERMINE RECTANGLE FROM POINT.
    return QRect(0, 0, 1, 1);
}

/*
 * Method: scrollTo
 */
void LineGraphView::scrollTo(const QModelIndex &index, ScrollHint)
{
  // DO WE NEED TO SCROLL?  THE GRAPH SHOULD OCCUPY THE FULL WINDOW.
  if (index.isValid())
    return;
}

/*
 * Method: indexAt
 */
QModelIndex LineGraphView::indexAt(const QPoint &point) const
{
  // LOOK IN HASH FOR INDEX.
  if (!point.isNull())
    return QModelIndex();
  else
    return QModelIndex();
}

/*
 * Method: eventFilter
 */
bool LineGraphView::eventFilter(QObject *object, QEvent *event)
{
  if ((object == view) && (event->type() == QEvent::Resize))
  {
    view->fitInView(sceneRectangle);
    return false;
  }
  else
  {
    return QAbstractItemView::eventFilter(object, event);
  }
}

/*
 * Method: moveCursor
 */
QModelIndex LineGraphView::moveCursor(CursorAction cursorAction,
                                      Qt::KeyboardModifiers)
{
  // FIND MODEL INDEX OF POINT UNDER CURSOR.
  switch (cursorAction)
  {
    default:
      return QModelIndex();
      break;
  }
}

/*
 * Method: horizontalOffset
 */
int LineGraphView::horizontalOffset() const
{
  // HORIZONTAL SCROLL BAR VALUE.
  return 0;
}

/*
 * Method: verticalOffset
 */
int LineGraphView::verticalOffset() const
{
  // VERTICAL SCROLL BAR VALUE.
  return 0;
}

/*
 * Method: setSelection
 */
void LineGraphView::setSelection(const QRect &rect,
                                 QItemSelectionModel::SelectionFlags flags)
{
  // SET SELECTED ITEMS.
  if (rect.isValid())
    selectionModel()->select(QModelIndex(), flags);
}

/*
 * Method: visualRegionForSelection
 */
QRegion LineGraphView::visualRegionForSelection(const QItemSelection
                                                    &selection) const
{
  // COMPUTE REGION CONTAINING SELECTION.
  if (selection.count())
    return QRegion();
  return QRegion();
}

/*
 * Method: isIndexHidden
 */
bool LineGraphView::isIndexHidden(const QModelIndex &index) const
{
  // DETERMINES IF ITEM IS HIDDEN.
  if (!index.isValid())
    return true;
  return false;
}

/*
 * Method: dataChanged
 */
void LineGraphView::dataChanged(const QModelIndex &topLeft,
                                const QModelIndex &bottomRight,
                                const QVector<int> &/*roles*/)
{
  // Expect only single instances to be modified.
  // This is consistent with the interface.
  for (int row = topLeft.row(); row <= bottomRight.row(); row++)
  {
    QModelIndex xIndex = model()->index(row, 0);
    QModelIndex yIndex = model()->index(row, 1);
    double xVal = model()->data(xIndex).toDouble();
    double yVal = model()->data(yIndex).toDouble();
    QPointF newPoint = QPointF(xVal, yVal);
    indexPointHash.insert(xIndex, newPoint);
  }

  // Recompute path; this is an unfortunate limitation of the QPainterPath.
  redrawPath();
}

/*
 * Method: rowsAboutToBeRemoved
 */
void LineGraphView::rowsAboutToBeRemoved(const QModelIndex &/*parent*/,
                                         int start, int end)
{
  for (int i = start; i <= end; i++)
  {
    QModelIndex index = model()->index(i, 0);
    indexPointHash.remove(index);
  }
  redrawPath();
}

/*
 * Method: redrawPath
 */
void LineGraphView::redrawPath()
{
  // Need to recompute entire path; can't just add/modify/remove one object.
  QPainterPath path;
  double minX, minY, maxX, maxY;

  // Sort points; find minimum and maximum x and y.
  QMap<double,double> points;
  QHash<QModelIndex,QPointF>::const_iterator i = indexPointHash.constBegin();
  minX = maxX = i.value().x();
  minY = maxY = i.value().y();

  while (i != indexPointHash.constEnd())
  {
    double x = i.value().x();
    double y = i.value().y();

    if (x < minX)
      minX = x;
    if (x > maxX)
      maxX = x;

    if (y < minY)
      minY = y;
    if (y > maxY)
      maxY = y;

    points.insert(x, y);
    i++;
  }

  //Add points to path in order.
  QMap<double,double>::const_iterator j = points.constBegin();
  path.moveTo(QPointF(j.key(), j.value()));

  while (j != points.constEnd())
  {
    path.lineTo(QPointF(j.key(), j.value()));
    j++;
  }

  // Set scene properties; draw connected line.
  sceneRectangle = QRectF(minX, minY, (maxX - minX),
                          (maxY - minY));
  scene->clear();
  scene->setSceneRect(sceneRectangle);

  QPen pen = QPen(Qt::SolidLine);
  pen.setCapStyle(Qt::RoundCap);
  pen.setJoinStyle(Qt::MiterJoin);
  pen.setWidthF(0);
  pen.setColor(QColor(255, 0, 0));
  scene->addPath(path, pen);

  // Draw axes.
  pen.setColor(QColor(0, 0, 0));
  scene->addLine(minX, 0, maxX, 0, pen);
  scene->addLine(0, minY, 0, maxY, pen);

  double stepH = (maxX - minX) / 10.0;
  double stepV = (maxY - minY) / 10.0;
  for (double pos = minX; pos < maxX; pos += stepH)
  {
    scene->addLine(pos, stepV / -10.0,
                   pos, stepV / 10.0, pen);
  }
  xLabel->setText("XLabel Text");

  for (double pos = minY; pos < maxY; pos += stepV)
  {
    scene->addLine(stepH / -10.0, pos,
                   stepH / 10.0, pos, pen);
  }
  yLabel->setText("YLabel Text");

  view->setScene(scene);
  view->fitInView(sceneRectangle);
  view->show();
}

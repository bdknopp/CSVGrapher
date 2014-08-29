/*
 * LineGraphView.h: Line-graph view into data model.
 * Author: B. D. Knopp: bdknopp@users.noreply.github.com
 * Version: .99: Initial implementation; simplistic view.
 * Date: 25 August 2014
 */

#ifndef LINEGRAPHVIEW_H
#define LINEGRAPHVIEW_H

/* C includes. */
#include <cmath>

/* Qt includes. */
#include <QAbstractItemView>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>

#include <QEvent>

/*
 * Class: LineGraphView
 * Description: Provides a line-graph view into associated data model.
 */
class LineGraphView : public QAbstractItemView
{
  Q_OBJECT

  /* Public methods. */
  public:
    /*
     * Constructor: LineGraphView
     * Description: Constructs a new LineGraphView, unassociated with
     *            : any model.
     * Parameters: parent: Parent object to associate with; default 0.
     */
    explicit LineGraphView(QWidget *parent = 0);

    /*
     * Destructor: ~LineGraphView
     * Description: Destroys associated QGraphicsScene with instance.
     */
    ~LineGraphView();

    /*
     * Method: setGraphicsView
     * Description: Links existing Ui graphics view element to this view.
     *            : This QGraphicsView will be drawn upon.
     * Parameters: graphicsView: View to draw line graph on.
     * Returns: none.
     */
    void setGraphicsView(QGraphicsView *graphicsView);

    /*
     * Method: setLabels
     * Description: Links existing Ui labels to this view; for display of
     *            : graph axis labels.
     * Parameters: xLabel, yLabel: QLabel for X- and Y-axes.
     * Returns: none.
     */
    void setLabels(QLabel *xLabel, QLabel *yLabel);

    /*
     * Method: visualRect
     * Description: Determines rectangle on screen which item occupies.
     * Parameters: index: Index of item to find coordinates of.
     * Returns: Rectange in which item denoted by index is.
     */
    QRect visualRect(const QModelIndex &index) const;

    /*
     * Method: scrollTo
     * Description: Scrolls view to ensure item at index is visible.
     * Parameters: index: Index of item to ensure is visible.
     *           : hint: Scrolling hint to assist.
     * Returns: none.
     */
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);

    /*
     * Method: indexAt
     * Description: Determines the model index of the item at point.
     * Parameters: point: Point in view coordinates to find model index of.
     * Returns: Model index of object at point.
     */
    QModelIndex indexAt(const QPoint &point) const;

  /* Protected methods. */
  protected:
    /*
     * Method: eventFilter
     * Description: Handles redrawing on resize for graphics view.
     * Parameters: object: Object triggering event.
     *           : event: Event to filter.
     * Returns: none.
     */
    bool eventFilter(QObject *object, QEvent *event);

    /*
     * Method: moveCursor
     * Description: Finds the model index of the next item, determined
     *            : from cursor action and keyboard modifiers.
     * Parameters: cursorAction, modifiers: Mouse and keyboard actions to
     *           :                        : assist in determining index.
     * Returns: Model index of next item.
     */
    QModelIndex moveCursor(CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers);

    /*
     * Method: horizontalOffset
     * Description: Finds the horizontal offset of the view.
     * Parameters: none.
     * Returns: Horizontal offset of view.
     */
    int horizontalOffset() const;

    /*
     * Method: verticalOffset
     * Description: Finds the vertical offset of the view.
     * Parameters: none.
     * Returns: Vertical offset of view.
     */
    int verticalOffset() const;

    /*
     * Method: isIndexHidden
     * Description: Determines if the item at model index is not visible.
     * Arguments: index: Model index of item.
     * Returns: True if item at index is hidden; false otherwise.
     */
    bool isIndexHidden(const QModelIndex &index) const;

    /*
     * Method: setSelection
     * Description: Selects all items in the selection rectangle.
     * Parameters: rect, flags: Rectangle to select items in; flags to apply
     *           :            : to selection.
     * Returns: none.
     */
    void setSelection(const QRect &rect,
                      QItemSelectionModel::SelectionFlags flags);

    /*
     * Method: visualRegionForSelection
     * Description: Determines region correspoding to selection.
     * Parameters: selection: Item selection to create region for.
     * Returns: Region containing item selection.
     */
    QRegion visualRegionForSelection(const QItemSelection &selection) const;

  /* Signals. */
  signals:
    /* None. */

  /* Slots. */
  protected slots:
    /*
     * Method: dataChanged
     * Description: Called when data in the model is modified; redraws
     *            : line graph.
     * Parameters: topLeft, bottomRight: Indices indicating range of modified
     *           :                     : data.
     * Returns: none.
     */
    void dataChanged(const QModelIndex &topLeft,
                     const QModelIndex &bottomRight,
                     const QVector<int> &roles = QVector<int>());

    /*
     * Method: rowsAboutToBeRemoved
     * Description: Called when rows are going to be removed; removes those
     *            : points on graph.
     * Parameters: parent: Parent index.
     *           : start, end: Beginning and ending row indices.
     * Returns: none.
     */
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

    /*
     * Method: rowsInserted
     * Description: Called when rows are added; adds those points on graph.
     * Parameters: parent: Parent index.
     *           : start, end: Beginning and ending row indices.
     * Returns: none.
     */
    //void rowsInserted(const QModelIndex &parent, int start, int end);

  /* Private members. */
  private:
    /*
     * Method: redrawPath
     * Description: Recomputes and redraws the graphics line path.
     * Parameters: none.
     * Returns: none.
     */
    void redrawPath();

    QRectF sceneRectangle;
    QGraphicsView *view;
    QGraphicsScene *scene;

    QLabel *xLabel, *yLabel;

    // Map of model indices to graphics objects.
    QHash<QModelIndex,QPointF> indexPointHash;
};

#endif // LINEGRAPHVIEW_H

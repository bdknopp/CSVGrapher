/*
 * MainWindow.cpp: See "MainWindow.h" for documentation.
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

/*
 * Constructor: MainWindow
 */
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  dataModel(new QStandardItemModel(this))
{
  ui->setupUi(this);
  initializeModel("X-data", "Y-data", QStringList());
  initializeViews();
}

/*
 * Destructor: ~MainWindow
 */
MainWindow::~MainWindow()
{
  delete ui;
  delete dataModel;
  delete graphView;
}

/*
 * Method: on_browseButton_clicked
 * Description: Browses for a file to open or save.
 * Parameters: none.
 * Returns: none.
 */
void MainWindow::on_browseButton_clicked()
{
  QString fName = QFileDialog::getOpenFileName(this, tr("Select file..."),
                                               "~/", tr("CSV File (*.csv)"));
  ui->fileTextBox->setText(fName);
}

/*
 * Method: on_fileOpenButton_clicked
 */
void MainWindow::on_fileOpenButton_clicked()
{
  // Try to read; display error if failed.
  try
  {
    readCSVFile(ui->fileTextBox->text());
  }
  catch (CSVFileException csvFExc)
  {
    QErrorMessage error;
    error.showMessage(csvFExc.what());
    error.exec();
  }
}

/*
 * Method: on_fileSaveButton_clicked
 */
void MainWindow::on_fileSaveButton_clicked()
{
  // Try to write; display error if failed.
  try
  {
    writeCSVFile(ui->fileTextBox->text());
  }
  catch (CSVFileException csvFExc)
  {
    QErrorMessage error;
    error.showMessage(csvFExc.what());
    error.exec();
  }
}

/*
 * Method: on_addRowButton_clicked
 */
void MainWindow::on_addRowButton_clicked()
{
  // Add single row below currently selected one.
  //   Or, add at beginning.
  QModelIndexList rowList = selectionModel->selectedRows();
  if (rowList.size() > 0)
    dataModel->insertRows(rowList.at(0).row(), 1);
  else
    dataModel->insertRows(0, 1);
}

/*
 * Method: on_removeRowButton_clicked
 */
void MainWindow::on_deleteRowButton_clicked()
{
  // Get list of selected rows; delete each one.
  QModelIndexList rowList = selectionModel->selectedRows();
  if (rowList.size() > 0)
    dataModel->removeRows(rowList.at(0).row(), rowList.size());
}

/*
 * Method: readCSVFile
 */
void MainWindow::readCSVFile(QString fName) throw(CSVFileException)
{
  QFile inFile(fName);
  if (inFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QTextStream inStream(&inFile);
    QString xLabel, yLabel;
    QStringList lines;

    // Read header information on first line.
    if (!inStream.atEnd())
    {
      QStringList labels = inStream.readLine().split(",");

      // Error if more or less than two columns.
      if (labels.size() != 2)
      {
        std::string msg = "File \"" + fName.toStdString() +
            "\" incorrectly formatted.";
        throw CSVFileException(msg);
      }

      xLabel = labels.at(0);
      yLabel = labels.at(1);
    }

    // Read in all data lines.
    while (!inStream.atEnd())
      lines << inStream.readLine();

    initializeModel(xLabel, yLabel, lines);
  }
  else
  {
    // Couldn't open file; abort with exception.
    std::string msg = "Cannot open file \"" + fName.toStdString() +
        "\" for reading.";
    throw CSVFileException(msg);
  }

  inFile.close();
}

/*
 * Method: writeCSVFile
 */
void MainWindow::writeCSVFile(QString fName) throw(CSVFileException)
{
  QFile outFile(fName);
  if (outFile.open(QFile::WriteOnly | QFile::Truncate |
                   QFile::Text))
  {
    // Write header information on first line.
    QTextStream outStream(&outFile);
    QString xLabel = dataModel->headerData(0, Qt::Horizontal).toString();
    QString yLabel = dataModel->headerData(1, Qt::Horizontal).toString();
    outStream << xLabel << "," << yLabel << "\n";

    // Write lines of data.
    for (int i = 0; i < dataModel->rowCount(); i++)
    {
      QString xItem = dataModel->data(dataModel->index(i, 0, QModelIndex()),
                                      Qt::DisplayRole).toString();
      QString yItem = dataModel->data(dataModel->index(i, 1, QModelIndex()),
                                      Qt::DisplayRole).toString();
      outStream << xItem << "," << yItem << "\n";
    }
  }
  else
  {
    // Couldn't open file; abort with exception.
    std::string msg = "Cannot open file \"" + fName.toStdString() +
        "\" for writing.";
    throw CSVFileException(msg);
  }

  outFile.close();
}

/*
 * Method: initializeModel
 */
void MainWindow::initializeModel(QString xLabel, QString yLabel,
                                 QStringList dataLines)
{
  dataModel->setRowCount(dataLines.size());
  dataModel->setColumnCount(2);
  dataModel->setHeaderData(0, Qt::Horizontal, xLabel);
  dataModel->setHeaderData(1, Qt::Horizontal, yLabel);

  for (int i = 0; i < dataLines.size(); i++)
  {
    QStringList data = dataLines.at(i).split(",");
    dataModel->setData(dataModel->index(i, 0, QModelIndex()),
                       data.at(0).toDouble());
    dataModel->setData(dataModel->index(i, 1, QModelIndex()),
                       data.at(1).toDouble());
  }
}

/*
 * Method: initializeViews
 */
void MainWindow::initializeViews()
{
  // Line graph view.
  graphView = new LineGraphView();
  graphView->setModel(dataModel);
  graphView->setGraphicsView(ui->graphicsView);
  //graphView->setLabels(ui->xLabel, ui->yLabel);

  selectionModel = new QItemSelectionModel(dataModel);
  ui->tableView->setModel(dataModel);
  ui->tableView->setSelectionModel(selectionModel);
}

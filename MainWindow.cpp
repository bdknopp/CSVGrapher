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
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

/*
 * Destructor: ~MainWindow
 */
MainWindow::~MainWindow()
{
  delete ui;
}

/*
 * Method: on_fileOpenButton_clicked
 */
void MainWindow::on_fileOpenButton_clicked()
{
  QString fName = QFileDialog::getOpenFileName(this, tr("Open Data File"),
                                               "~/", tr("CSV File (*.csv)"));
  ui->fileTextBox->setText(fName);

  // Try to read; display error if failed.
  try
  {
    readCSVFile(fName);
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
  QString fName = QFileDialog::getOpenFileName(this, tr("Save Data File"),
                                               "~/", tr("CSV File (*.csv)"));
  ui->fileTextBox->setText(fName);

  // Try to write; display error if failed.
  try
  {
    writeCSVFile(fName);
  }
  catch (CSVFileException csvFExc)
  {
    QErrorMessage error;
    error.showMessage(csvFExc.what());
    error.exec();
  }
}

/*
 * Method: on_valueAddButton_clicked
 */
void MainWindow::on_valueAddButton_clicked()
{

}

/*
 * Method: on_valueModifyButton_clicked
 */
void MainWindow::on_valueModifyButton_clicked()
{

}

/*
 * Method: on_valueRemoveButton_clicked
 */
void MainWindow::on_valueRemoveButton_clicked()
{

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
  dataModel = new QStandardItemModel(dataLines.size(), 2, this);
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

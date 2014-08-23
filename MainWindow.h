/*
 * MainWindow.h: GUI Control component; provides controls to modify the data
 *             : Model.
 * Author: B. D. Knopp: bdknopp@users.noreply.github.com
 * Version: 1.00: Initial implementation.
 * Date: 22 August 2014
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* C++ includes. */
#include <exception>

/* Qt includes. */
#include <QMainWindow>
#include <QFileDialog>
#include <QErrorMessage>

#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <QStandardItemModel>

/* Project includes. */
#include "CSVFileException.h"

/*
 * Namespace: Ui
 * Description: Encapsulates Ui View components.
 */
namespace Ui {
  class MainWindow;
}

/*
 * Class: MainWindow
 * Description: Primary graphical viewing window; drawing of graph from model,
 *            : reading/writing from/to files, and provision of controls.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

  /* Public methods. */
  public:
    /*
     * Constructor: MainWindow
     * Description: Instantiates the main view window.
     * Parameters: parent: ID of parent object; default 0.
     */
    explicit MainWindow(QWidget *parent = 0);

    /*
     * Destructor: ~MainWindow
     * Description: Destroys instance of MainWindow.
     */
    ~MainWindow();

  /* Private slot methods. */
  private slots:
    /*
     * Method: on_fileOpenButton_clicked
     * Description: Opens data file from name, populating data model.
     * Parameters: none.
     * Returns: none.
     */
    void on_fileOpenButton_clicked();

    /*
     * Method: on_fileSaveButton_clicked
     * Description: Saves data model to file from name.
     * Parameters: none.
     * Returns: none.
     */
    void on_fileSaveButton_clicked();

    /*
     * Method: on_valueAddButton_clicked
     * Description: Adds a new value to the data model.
     * Parameters: none.
     * Returns: none.
     */
    void on_valueAddButton_clicked();

    /*
     * Method: on_valueModifyButton_clicked
     * Description: Modifies the selected value in the data model.
     * Parameters: none.
     * Returns: none.
     */
    void on_valueModifyButton_clicked();

    /*
     * Method: on_valueRemoveButton_clicked
     * Description: Removes the selected value from the data model.
     * Parameters: none.
     * Returns: none.
     */
    void on_valueRemoveButton_clicked();

  /* Private members. */
  private:
    /*
     * Method: readCSVFile
     * Description: Reads the data contained in the named CSV file, placing it
     *            : into model.
     * Parameters: fName: Name of CSV file to read.
     * Returns: none.
     */
    void readCSVFile(QString fName) throw(CSVFileException);

    /*
     * Method: writeCSVFile
     * Description: Writes the data contained in the model to the named CSV
     *            : file.
     * Parameters: fName: Name of CSV file to write to.
     * Returns: none.
     */
    void writeCSVFile(QString fName) throw(CSVFileException);

    /*
     * Method: initializeModel
     * Description: Populates the data model with labels and data.
     * Parameters: xLabel: X-axis label.
     *           : yLabel: Y-axis label.
     *           : dataLines: Lines of data to place in the model.
     * Returns: none.
     */
    void initializeModel(QString xLabel, QString yLabel,
                         QStringList dataLines);

    // Reference to main window.
    Ui::MainWindow *ui;

    // Data Model.
    QStandardItemModel *dataModel;
};

#endif // MAINWINDOW_H

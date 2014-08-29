CSVGrapher: Simple CSV table-based data editor and graph display.
Author: B. D. Knopp <bdknopp@users.noreply.github.com>
http://github.com/bdknopp/CSVGrapher

Compilation:
The recommended way to compile the software source in this project is to use
the in-built facilities of the Qt Creator IDE.  First, open the
'CSVGrapher.pro' project file in Qt Creator.  Then build the project by
navigating to "Build->Build All".  After building, you may run the project from
within the IDE by navigating to "Build->Run".

Installation:
The application doesn't strictly require installation into a system directory; 
it may be executed from any directory where the user has permission to execute
files from.

Usage:
CSVGrapher is interacted with through a Qt GUI.  The GUI allows users to select
two-column *.csv files for reading and writing.  Data read from the selected
file will be pulled into the programs internal data model.  This model will be
reflected by the table view, and by the accompanying graph view.

Users may modify existing data in the table, both independent and dependent
variables.  A user may also add or remove rows from the table.  Rows may be
added either at the beginning, or anywhere between existing rows by selecting
the row above which to add the new row.  Rows may be deleted by selecting
either one row or a range of rows.

The graph view is in the style of a line graph.  Axes are drawn and are divided
into ten gradations throughout the domain and range of the data.  Labels
appear below the graph view, stating the given units or interpretation of the
axis (as specified in the *.csv file) and the intervals into which the data are
divided (approximate number of units).

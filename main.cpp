/*
 * main.cpp: Main model-view-controller initialization routine.
 * Author: B. D. Knopp: bdknopp@users.noreply.github.com
 * Version: 1.00: Initial implementation.
 * Date: 22 August 2014
 */

/* Project includes. */
#include "MainWindow.h"

/* Qt includes. */
#include <QApplication>

/*
 * Procedure: main
 * Description: Initializes GUI view.
 * Parameters: argc: Argument count; unused.
 *           : argv: Argument vector; unused.
 * Returns: 0 if terminated cleanly.
 */
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}

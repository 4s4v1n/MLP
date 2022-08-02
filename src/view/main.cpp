/**
 * @file main.cpp
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#include <QApplication>

#include "main_window.h"

int main(int argc, char *argv[]) {
  setlocale(LC_NUMERIC, "C");
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":/icon.png"));
  MainWindow w;
  w.show();
  return a.exec();
}

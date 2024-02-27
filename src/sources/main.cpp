// #include <locale.h>

#include <QApplication>

#include "view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::Model m("0");
  s21::Controller c(&m);
  s21::View v(&c);
  v.show();
  return a.exec();
}

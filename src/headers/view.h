#ifndef VIEW_H
#define VIEW_H

#include <qcustomplot.h>

#include <QMainWindow>

#include "controller.h"

namespace s21 {

class View : public QMainWindow {
 private:
  double leftX = -10, rightX = 10;
  double leftY = -10, rightY = 10;
  double step = 0.1;

  Controller *controller;
  QCustomPlot *wGraphic;

  // dockWidgetTop widgets
  QDockWidget *dockWidgetTop;
  QWidget *widget_dwt;
  QHBoxLayout *layout_dwt;
  QLineEdit *lineEdit_func;

  // dockWidgetBottom widgets
  QDockWidget *dockWidgetBottom;
  QWidget *widget_dwb;
  QHBoxLayout *layout_dwb;
  QLineEdit *lineEdit_leftX;
  QLineEdit *lineEdit_rightX;
  QLineEdit *lineEdit_leftY;
  QLineEdit *lineEdit_rightY;

  QStatusBar *statusBar;

  void createWidgets();
  void createDockWidgetTop();
  void createDockWidgetBottom();
  void settingWidgets();
  void callingSlots();
  void drawGraph();
  void checkAreaXY();

 private slots:
  void settingGraph();

 public:
  View(Controller *c = nullptr, QWidget *parent = nullptr);
  ~View(){};
};

}  // namespace s21
#endif  // VIEW_H
#ifndef VIEW_H
#define VIEW_H

#include <qcustomplot.h>

#include <QMainWindow>

#include "controller.h"

namespace s21 {

#define Xmin -1000000
#define Xmax 1000000
#define Ymin -1000000
#define Ymax 1000000

class View : public QMainWindow {
 private:
  double leftX = -10, rightX = 10;
  double step;

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

  QStatusBar *statusBar;

  void createWidgets();
  void createDockWidgetTop();
  void createDockWidgetBottom();
  void settingWidgets();
  void callingSlots();
  void drawGraph();
  void checkAreaXY();
  bool isBreakpoint(QVector<double> &y);
  void correctionStep(double X, double Y);

 private slots:
  void settingGraph();

 public:
  View(QWidget *parent = nullptr, Controller *c = nullptr);
  ~View(){};
};

}  // namespace s21
#endif  // VIEW_H
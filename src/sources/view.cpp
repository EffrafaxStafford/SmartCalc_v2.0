#include "view.h"

#include <iostream>

s21::View::View(QWidget *parent, Controller *c)
    : QMainWindow(parent), controller(c) {
  this->resize(600, 500);

  createWidgets();
  settingWidgets();

  setCentralWidget(wGraphic);
  addDockWidget(Qt::TopDockWidgetArea, dockWidgetTop);
  addDockWidget(Qt::BottomDockWidgetArea, dockWidgetBottom);
  setStatusBar(statusBar);

  callingSlots();
}

void s21::View::callingSlots() {
  connect(lineEdit_func, &QLineEdit::editingFinished, this,
          &View::settingGraph);
  connect(lineEdit_leftX, &QLineEdit::editingFinished, this,
          &View::settingGraph);
  connect(lineEdit_rightX, &QLineEdit::editingFinished, this,
          &View::settingGraph);
}

void s21::View::createWidgets() {
  wGraphic = new QCustomPlot(this);
  createDockWidgetTop();
  createDockWidgetBottom();
  statusBar = new QStatusBar(this);
}

void s21::View::createDockWidgetTop() {
  dockWidgetTop = new QDockWidget(this);
  widget_dwt = new QWidget(this);
  layout_dwt = new QHBoxLayout(widget_dwt);
  dockWidgetTop->setTitleBarWidget(widget_dwt);

  lineEdit_func = new QLineEdit(this);
  layout_dwt->addWidget(new QLabel("y(x): ", this));
  layout_dwt->addWidget(lineEdit_func);
}

void s21::View::createDockWidgetBottom() {
  dockWidgetBottom = new QDockWidget(this);
  widget_dwb = new QWidget(this);
  layout_dwb = new QHBoxLayout(widget_dwb);
  dockWidgetBottom->setTitleBarWidget(widget_dwb);

  lineEdit_leftX = new QLineEdit("-10", this);
  lineEdit_rightX = new QLineEdit("10", this);

  layout_dwb->addWidget(new QLabel("x \u2208 [", this));
  layout_dwb->addWidget(lineEdit_leftX);
  layout_dwb->addWidget(new QLabel(";", this));
  layout_dwb->addWidget(lineEdit_rightX);
  layout_dwb->addWidget(new QLabel("]", this));
}

void s21::View::settingWidgets() {
  dockWidgetTop->setFeatures(QDockWidget::NoDockWidgetFeatures);
  dockWidgetBottom->setFeatures(QDockWidget::NoDockWidgetFeatures);

  QIntValidator *validator = new QIntValidator(this);
  lineEdit_leftX->setValidator(validator);
  lineEdit_rightX->setValidator(validator);

  wGraphic->xAxis->setRange(leftX, rightX);
  wGraphic->yAxis->setRange(-10, 10);
  wGraphic->xAxis->setLabel("x");
  wGraphic->yAxis->setLabel("y");
  wGraphic->setInteraction(QCP::iRangeDrag, true);
  wGraphic->setInteraction(QCP::iRangeZoom, true);

  wGraphic->axisRect()->setRangeDrag(Qt::Vertical);
  wGraphic->axisRect()->setRangeZoom(Qt::Vertical);
}

void s21::View::drawGraph() {
  std::string func = lineEdit_func->text().toStdString();
  controller->recalculate(func);
  double X = leftX;
  QVector<QVector<double>> x, y;
  for (int i = 0; X <= rightX; i++) {
    QVector<double> xi, yi;
    x.push_back(xi);
    y.push_back(yi);
    for (; X <= rightX; X += step) {
      controller->recalculate(X);
      if (isBreakpoint(y[i])) break;
      x[i].push_back(X);
      y[i].push_back(controller->getResult());
    }
    wGraphic->addGraph();
    wGraphic->graph(i)->addData(x[i], y[i]);
  }
  wGraphic->replot();
}

void s21::View::settingGraph() {
  try {
    leftX = lineEdit_leftX->text().toDouble();
    rightX = lineEdit_rightX->text().toDouble();
    checkAreaXY();
    step = (rightX - leftX) / 1000;
    wGraphic->xAxis->setRange(leftX, rightX);
    wGraphic->clearGraphs();
    wGraphic->replot();

    statusBar->clearMessage();
    drawGraph();
    if (!controller->x_is_there())
      statusBar->showMessage(QString::number(controller->getResult()));
  } catch (std::exception &ex) {
    statusBar->showMessage(tr(ex.what()));
  }
}

void s21::View::checkAreaXY() {
  if (leftX >= rightX || leftX < -1000000 || rightX > 1000000)
    throw std::runtime_error(
        "Incorrect input: check the function definition area.");
}

bool s21::View::isBreakpoint(QVector<double> &y) {
  return (!y.empty() && (abs(controller->getResult() - y.back()) > 10) &&
          ((controller->getResult() < 0 && y.back() > 0) ||
           (controller->getResult() > 0 && y.back() < 0)));
}

#include "view.h"

#include <iostream>

s21::View::View(Controller *c, QWidget *parent)
    : controller(c), QMainWindow(parent) {
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
  connect(lineEdit_leftY, &QLineEdit::editingFinished, this,
          &View::settingGraph);
  connect(lineEdit_rightX, &QLineEdit::editingFinished, this,
          &View::settingGraph);
  connect(lineEdit_rightY, &QLineEdit::editingFinished, this,
          &View::settingGraph);
}

void s21::View::createWidgets() {
  wGraphic = new QCustomPlot(this);
  wGraphic->addGraph();
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
  lineEdit_leftY = new QLineEdit("-10", this);
  lineEdit_rightY = new QLineEdit("10", this);

  layout_dwb->addWidget(new QLabel("x \u2208 [", this));
  layout_dwb->addWidget(lineEdit_leftX);
  layout_dwb->addWidget(new QLabel(";", this));
  layout_dwb->addWidget(lineEdit_rightX);
  layout_dwb->addWidget(new QLabel("],", this));

  layout_dwb->addWidget(new QLabel("y \u2208 [", this));
  layout_dwb->addWidget(lineEdit_leftY);
  layout_dwb->addWidget(new QLabel(";", this));
  layout_dwb->addWidget(lineEdit_rightY);
  layout_dwb->addWidget(new QLabel("]", this));
}

void s21::View::settingWidgets() {
  dockWidgetTop->setFeatures(QDockWidget::NoDockWidgetFeatures);
  dockWidgetBottom->setFeatures(QDockWidget::NoDockWidgetFeatures);

  QIntValidator *validator = new QIntValidator(this);
  lineEdit_leftX->setValidator(validator);
  lineEdit_rightX->setValidator(validator);
  lineEdit_leftY->setValidator(validator);
  lineEdit_rightY->setValidator(validator);

  wGraphic->xAxis->setRange(leftX, rightX);
  wGraphic->yAxis->setRange(leftY, rightY);
  wGraphic->xAxis->setLabel("x");
  wGraphic->yAxis->setLabel("y");
}

void s21::View::drawGraph() {
  statusBar->clearMessage();
  wGraphic->graph(0)->data()->clear();
  wGraphic->replot();

  wGraphic->xAxis->setRange(leftX, rightX);
  wGraphic->yAxis->setRange(leftY, rightY);
  std::string func = lineEdit_func->text().toStdString();
  controller->recalculate(func);

  QVector<double> x, y;
  for (double X = leftX; X < rightX; X += step) {
    controller->recalculate(X);
    x.push_back(X);
    y.push_back(controller->getResult());
  }
  wGraphic->graph(0)->addData(x, y);
  wGraphic->replot();

  if (!controller->x_is_there())
    statusBar->showMessage(QString::number(controller->getResult()));
}

void s21::View::settingGraph() {
  try {
    leftX = lineEdit_leftX->text().toDouble();
    rightX = lineEdit_rightX->text().toDouble();
    leftY = lineEdit_leftY->text().toDouble();
    rightY = lineEdit_rightY->text().toDouble();
    checkAreaXY();
    step = (rightX - leftX) / 1000;
    drawGraph();
  } catch (std::exception &ex) {
    statusBar->showMessage(tr(ex.what()));
  }
}

void s21::View::checkAreaXY() {
  if (leftX >= rightX || leftX < -1000000 || rightX > 1000000)
    throw std::runtime_error(
        "Incorrect input: check the function definition area.");

  if (leftY >= rightY || leftY < -1000000 || rightY > 1000000)
    throw std::runtime_error(
        "Incorrect input: check the scope of the function values.");
}
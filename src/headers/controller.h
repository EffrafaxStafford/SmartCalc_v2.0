#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"

namespace s21 {

class Controller {
 private:
  Model *model;

 public:
  Controller(Model *m) : model(m){};
  double getResult() { return model->getResult(); };
  void recalculate(double new_x) { model->recalculate(new_x); };
  void recalculate(std::string str) { model->recalculate(str); };
  std::string getPostfixNotation() { return model->getPostfixNotation(); };
  bool x_is_there() { return model->x_is_there(); };
};

}  // namespace s21
#endif  // CONTROLLER_H
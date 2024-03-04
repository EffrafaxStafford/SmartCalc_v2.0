#ifndef MODEL_H
#define MODEL_H

#include <cmath>
#include <iostream>
#include <map>
#include <stack>
#include <string>

namespace s21 {

class Model {
 private:
  std::string infixNotation;
  std::string postfixNotation;
  double x;
  double result;
  std::map<char, int> operatorPriority{
      {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'m', 2}, {'^', 3}, {'#', 4},
      {'~', 4}, {'c', 5}, {'s', 5}, {'t', 5}, {'C', 5}, {'S', 5}, {'T', 5},
      {'q', 5}, {'l', 5}, {'L', 5}, {'(', 6}, {')', 6}};
  // 'm' - mod, '^' - extent, '#' - unary plus, '~' - unary minus
  // 'c' - cos, 's' - sin, 't' - tan, 'C' - acos, 'S' - asin, 'T' - atan
  // 'q' - sqrt, 'l' - ln, 'L' - log

  void convertUnaryOperators();
  void unaryOperator(size_t pos);
  void changeSubst(const std::string substr1, const std::string substr2);
  void convertInfixNotation();
  void checkSyntax();
  void checkBracket();
  void checkInfixNotation();
  void toPostfixNotation();
  void fromStackToStr(std::stack<char> &stack);
  void calculation();
  void calcOperator(std::stack<double> &stack, char symbol);
  void calcUnaryOperator(std::stack<double> &stack, char symbol);
  void calcBinaryOperator(std::stack<double> &stack, char symbol);

 public:
  Model(std::string str = "0", double x_ = 0);
  std::string getPostfixNotation() { return postfixNotation; };
  void recalculate(double new_x);
  void recalculate(std::string str, double new_x = 0);
  double getResult() { return result; };
  bool x_is_there();
};

}  // namespace s21

#endif  //  MODEL_H
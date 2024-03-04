#include "../headers/model.h"

s21::Model::Model(std::string str, double x_) : infixNotation(str), x(x_) {
  if (str.c_str() == NULL || str == "")
    throw std::runtime_error("Incorrect input: enter the function y(x).");
  convertInfixNotation();
  checkInfixNotation();
  toPostfixNotation();
  calculation();
};

void s21::Model::convertUnaryOperators() {
  unaryOperator(0);
  for (size_t i = 0; i < infixNotation.length(); i++)
    if (infixNotation[i] == '(' || infixNotation[i] == '^')
      unaryOperator(i + 1);
}

void s21::Model::unaryOperator(size_t pos) {
  if (infixNotation[pos] == '-')
    infixNotation[pos] = '~';
  else if (infixNotation[pos] == '+')
    infixNotation[pos] = '#';
}

void s21::Model::changeSubst(const std::string substr1,
                             const std::string substr2) {
  while (infixNotation.find(substr1) != std::string::npos)
    infixNotation.replace(infixNotation.find(substr1), substr1.length(),
                          substr2);
}

void s21::Model::convertInfixNotation() {
  convertUnaryOperators();
  changeSubst("acos", "C");
  changeSubst("asin", "S");
  changeSubst("atan", "T");
  changeSubst("cos", "c");
  changeSubst("sin", "s");
  changeSubst("tan", "t");
  changeSubst("sqrt", "q");
  changeSubst("ln", "l");
  changeSubst("log", "L");
  changeSubst("mod", "m");
}

void s21::Model::checkSyntax() {
  // checkSyntax() is correct after convertInfixNotation()
  if (infixNotation.find_first_not_of("0123456789+-*/m^#~cstCSTqlL()eE.,x ") !=
      std::string::npos)
    throw std::runtime_error("Incorrect input: syntax error.");
}

void s21::Model::checkBracket() {
  unsigned char count = 0;
  for (size_t i = 0; i < infixNotation.length(); i++)
    if (infixNotation[i] == '(')
      count++;
    else if (infixNotation[i] == ')')
      count--;

  if (count) throw std::runtime_error("Incorrect input: check the brackets.");
}

void s21::Model::checkInfixNotation() {
  checkSyntax();
  checkBracket();
}

// Implementation of Dijkstra's algorithm
void s21::Model::toPostfixNotation() {
  std::stack<char> stack;

  for (size_t i = 0; i < infixNotation.length(); i++) {
    if (operatorPriority.find(infixNotation[i]) == operatorPriority.end()) {
      postfixNotation.push_back(infixNotation[i]);
    } else if (infixNotation[i] == ')') {
      while (stack.top() != '(') fromStackToStr(stack);
      stack.pop();
    } else if (i > 0 &&
               (infixNotation[i - 1] == 'e' || infixNotation[i - 1] == 'E')) {
      postfixNotation.push_back(infixNotation[i]);
    } else {
      postfixNotation.push_back(' ');
      while (!stack.empty() && stack.top() != '(' &&
             operatorPriority[stack.top()] >=
                 operatorPriority[infixNotation[i]])
        fromStackToStr(stack);
      stack.push(infixNotation[i]);
    }
  };
  while (!stack.empty()) fromStackToStr(stack);
}

void s21::Model::fromStackToStr(std::stack<char>& stack) {
  postfixNotation.push_back(' ');
  postfixNotation.push_back(stack.top());
  postfixNotation.push_back(' ');
  stack.pop();
}

void s21::Model::calculation() {
  std::stack<double> stack;
  std::string str =
      postfixNotation;  // избегаем изменения postfixNotation strtok()
  char* ptr = strtok((char*)str.c_str(), " ");

  while (ptr) {
    if (*ptr == 'x')
      stack.push(x);
    else if (*ptr >= '0' && *ptr <= '9')
      stack.push(std::atof(ptr));
    else
      calcOperator(stack, *ptr);
    ptr = strtok(NULL, " ");
  };
  if (stack.empty())
    throw std::runtime_error("Incorrect input: calculation error.");
  result = stack.top();
}

void s21::Model::calcOperator(std::stack<double>& stack, char symbol) {
  if (stack.empty())
    throw std::runtime_error("Incorrect input: calculation error.");

  if (std::string("cstCSTqlL~#").find(symbol) != std::string::npos)
    calcUnaryOperator(stack, symbol);
  else
    calcBinaryOperator(stack, symbol);
}

void s21::Model::calcUnaryOperator(std::stack<double>& stack, char symbol) {
  switch (symbol) {
    case ('c'):
      stack.top() = cos(stack.top());
      break;
    case ('s'):
      stack.top() = sin(stack.top());
      break;
    case ('t'):
      stack.top() = tan(stack.top());
      break;
    case ('C'):
      stack.top() = acos(stack.top());
      break;
    case ('S'):
      stack.top() = asin(stack.top());
      break;
    case ('T'):
      stack.top() = atan(stack.top());
      break;
    case ('q'):
      stack.top() = sqrt(stack.top());
      break;
    case ('l'):
      stack.top() = log(stack.top());
      break;
    case ('L'):
      stack.top() = log10(stack.top());
      break;
    case ('~'):
      stack.top() *= -1;
      break;
    case ('#'):
      break;
  };
}

void s21::Model::calcBinaryOperator(std::stack<double>& stack, char symbol) {
  double tmp = stack.top();
  stack.pop();
  if (stack.empty())
    throw std::runtime_error("Incorrect input: calculation error.");

  switch (symbol) {
    case ('+'):
      stack.top() += tmp;
      break;
    case ('-'):
      stack.top() -= tmp;
      break;
    case ('*'):
      stack.top() *= tmp;
      break;
    case ('/'):
      stack.top() /= tmp;
      break;
    case ('^'):
      stack.top() = std::pow(stack.top(), tmp);
      break;
    case ('m'):
      stack.top() = std::fmod(stack.top(), tmp);
      break;
  };
}

void s21::Model::recalculate(double new_x) {
  result = 0;
  this->x = new_x;
  calculation();
}

void s21::Model::recalculate(std::string str, double new_x) {
  if (str.c_str() == NULL || str == "")
    throw std::runtime_error("Incorrect input: enter the function y(x).");
  postfixNotation.clear();
  infixNotation = str;
  this->x = new_x;
  convertInfixNotation();
  checkInfixNotation();
  toPostfixNotation();
  calculation();
}

bool s21::Model::x_is_there() {
  return (infixNotation.find("x") != std::string::npos);
}
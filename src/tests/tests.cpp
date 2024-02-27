#include "../headers/model.h"
#include "gtest/gtest.h"

#define EPS 1e-7
using namespace s21;

TEST(toPostfixNotation, test1) {
  Model m1("1-2e-7*3");
  EXPECT_EQ(m1.getPostfixNotation(), "1 2e-7 3 *  - ");
  Model m2("1+(2+3e+13)*4");
  EXPECT_EQ(m2.getPostfixNotation(), "1  2 3e+13 +  4 *  + ");
  Model m3("1+2*(3e+4-4/(5+6))");
  EXPECT_EQ(m3.getPostfixNotation(), "1 2  3e+4 4  5 6 +  /  -  *  + ");
  Model m4("((1+2)*3)-4");
  EXPECT_EQ(m4.getPostfixNotation(), "  1 2 +  3 *  4 - ");
  Model m5("(1+2)*(3-4)");
  EXPECT_EQ(m5.getPostfixNotation(), " 1 2 +   3 4 -  * ");
  Model m6("((1+2)*3+4)/(5+6+7)");
  EXPECT_EQ(m6.getPostfixNotation(), "  1 2 +  3  * 4 +   5 6  + 7 +  / ");
}

TEST(calculation, test1) {
  Model m1("-x+3", 4.);
  EXPECT_NEAR(m1.getResult(), -1, EPS);
  Model m2("+(-x+3)", 4.);
  EXPECT_NEAR(m2.getResult(), -1, EPS);
  Model m3("2*1e+2");
  EXPECT_NEAR(m3.getResult(), 200, EPS);
}

TEST(calculation, test2) {
  Model m1(
      "15/(7-(1+1))*3-(2+(1+1))*15/(7-(200+1))*3-(2+(1+1))*(15/"
      "(7-(1+1))*3-(2+(1+1))+15/(7-(1+1))*3-(2+(1+1)))");
  EXPECT_NEAR(m1.getResult(), -30.0721649484536082, EPS);
  Model m2("2*ln(ln(ln((-1)*(-1)*x)))", 18);
  EXPECT_NEAR(m2.getResult(), 0.11914956265, EPS);
  Model m3("-x*cos(0)*sqrt(4)*log(10)*(-x)*x", 2);
  EXPECT_NEAR(m3.getResult(), 16, EPS);
  Model m4("sin(cos(12e-1^2*x))", 2);
  EXPECT_NEAR(m4.getResult(), -0.82260615913, EPS);
  Model m5("log(1e+28)");
  EXPECT_NEAR(m5.getResult(), 28, EPS);
  Model m6("5.34 mod 0.29");
  EXPECT_NEAR(m6.getResult(), 0.12, EPS);
}

TEST(calculation, testError) {
  try {
    Model m("1+2+r3");
  } catch (std::exception &ex) {
    EXPECT_EQ((std::string)ex.what(), "Incorrect input: syntax error.");
  }
  try {
    Model m("((1+2)");
  } catch (std::exception &ex) {
    EXPECT_EQ((std::string)ex.what(), "Incorrect input: check the brackets.");
  }
  try {
    Model m("()*6");
  } catch (std::exception &ex) {
    EXPECT_EQ((std::string)ex.what(), "Incorrect input: calculation error.");
  }
  try {
    Model m("((()*()))");
  } catch (std::exception &ex) {
    EXPECT_EQ((std::string)ex.what(), "Incorrect input: calculation error.");
  }
  try {
    Model m("");
  } catch (std::exception &ex) {
    EXPECT_EQ((std::string)ex.what(),
              "Incorrect input: enter the function y(x).");
  }
}

int main() {
  try {
    ::testing::InitGoogleTest();
  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
  }
  return RUN_ALL_TESTS();
}
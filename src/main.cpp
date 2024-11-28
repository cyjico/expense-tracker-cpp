#include "Application.h"
#include "pages/AddExpensePage.h"
#include "pages/HomePage.h"
#include <iostream>
#include <memory>

int main() {
  const PageMap pages = {
      {"/", std::make_shared<HomePage>(HomePage())},
      {"/add-expense", std::make_shared<AddExpensePage>(AddExpensePage())}};

  Application app(pages);
  app.RunIndefinitely();

  std::cout << "Reached end of program.";
  return 0;
}

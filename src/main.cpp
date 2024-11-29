#include "application.h"
#include "pages/add_expense_page.h"
#include "pages/home_page.h"
#include <iostream>
#include <memory>

int main() {
  application app;
  app.initialize({{"/", std::make_shared<home_page>(home_page(app))},
                  {"/add-expense",
                   std::make_shared<add_expense_page>(add_expense_page())}});
  app.run_indefinitely();

  std::cout << "Reached end of program.";
  return 0;
}

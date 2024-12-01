#include "application.h"
#include "pages/add_expense_page.h"
#include "pages/generate_report_page.h"
#include "pages/home_page.h"
#include "pages/search_expenses/by_category_page.h"
#include "pages/search_expenses_page.h"
#include "pages/view_expenses_page.h"
#include <iostream>
#include <memory>

int main() {
  application app;

  auto home_page_ptr = std::make_shared<home_page>(home_page());
  home_page_ptr->attach_listeners(app);

  auto view_expenses_page_ptr =
      std::make_shared<view_expenses_page>(view_expenses_page(4));
  view_expenses_page_ptr->attach_listeners(app);

  app.initialize({
      {"/", home_page_ptr},
      {"/add-expense", std::make_shared<add_expense_page>(add_expense_page())},
      {"/view-expenses", view_expenses_page_ptr},
      {"/generate-report",
       std::make_shared<generate_report_page>(generate_report_page())},
      {"/search-expenses",
       std::make_shared<search_expenses_page>(search_expenses_page())},
      {"/search-expenses/by-category",
       std::make_shared<search_expenses::by_category_page>(
           search_expenses::by_category_page())},
  });
  app.run_indefinitely();

  std::cout << "Reached end of program.";
  return 0;
}

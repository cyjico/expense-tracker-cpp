#include "application.h"
#include "pages/add_expense_page.h"
#include "pages/generate_report_page.h"
#include "pages/home_page.h"
#include "pages/search_expenses/by_category_page.h"
#include "pages/search_expenses/by_datetime_page.h"
#include "pages/search_expenses_page.h"
#include "pages/view_expenses_page.h"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

namespace {

[[noreturn]]
void main_terminate_handler() {
  std::cerr << "Unhandled exception! Terminating program." << '\n';

  // Output exception into an error.log file.
  try {
    const std::exception_ptr except = std::current_exception();
    if (except) {
      std::rethrow_exception(except);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';

    std::ofstream log_file("error.log", std::ios::app);
    if (log_file) {
      log_file << "Error: " << e.what() << '\n';
    }
  } catch (...) {
    std::cerr << "Error: Unknown exception type" << '\n';

    std::ofstream log_file("error.log", std::ios::app);
    if (log_file) {
      log_file << "Error: Unknown exception type\n";
    }
  }

  std::abort();
}

} // namespace

// NOLINTNEXTLINE(bugprone-exception-escape)
int main() {
  std::set_terminate(main_terminate_handler);
  application app;

  auto home_page_ptr = std::make_shared<home_page>();
  home_page_ptr->attach_listeners(app);

  auto view_expenses_page_ptr = std::make_shared<view_expenses_page>(4);
  view_expenses_page_ptr->attach_listeners(app);

  // Setup the page routes
  app.initialize(
      {{"/", home_page_ptr},
       {"/add-expense", std::make_shared<add_expense_page>()},
       {"/view-expenses", view_expenses_page_ptr},
       {"/generate-report", std::make_shared<generate_report_page>()},
       {"/search-expenses", std::make_shared<search_expenses_page>()},
       {"/search-expenses/by-category",
        std::make_shared<search_expenses::by_category_page>()},
       {"/search-expenses/by-datetime",
        std::make_shared<search_expenses::by_datetime_page>()}});
  app.run_indefinitely();

  std::cout << "Reached end of program.";
  return 0;
}

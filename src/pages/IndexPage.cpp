#include "pages/IndexPage.h"
#include "AbstractPage.h"
#include "Application.h"
#include <iostream>
#include <limits>

IndexPage::IndexPage() = default;

void IndexPage::Render(std::ostream &cout) {
  // \x1B      => ASCII escape character in hexadecimal
  // \x1B[2J   => Clear entire screen (J) from top to bottom (2)
  // \x1B[1;1H => Position cursor at 1st row (1) and 1st column (1H)
  cout << "\x1B[2J\x1B[1;1H";
  // For more info, see https://en.wikipedia.org/wiki/ANSI_escape_code

  cout << "1. Add Expense\n"
          "2. View Expenses\n"
          "3. Search Expenses by Category\n"
          "4. Generate Monthly Report\n"
          "5. Save and Exit\n";
}

UpdateAction IndexPage::Update(Application & /*app*/, std::istream &cin) {
  int inp = 0;
  cin >> inp;

  if (cin.fail()) {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return UpdateAction::RENDER;
  }

  constexpr int add_expense = 1;
  constexpr int view_expenses = 2;
  constexpr int search_expenses = 3;
  constexpr int generate_report = 4;
  constexpr int save_and_exit = 5;

  if (inp < add_expense || inp > save_and_exit) {
    return UpdateAction::RENDER;
  }

  switch (inp) {
  case add_expense:
    // Switch to add expense
    break;
  case view_expenses:
    // Switch to view expenses
    break;
  case search_expenses:
    // Switch to search expenses by category
    break;
  case generate_report:
    // Switch to generate monthly report
    break;
  case save_and_exit:
    return UpdateAction::EXIT;
  default:
    break;
  }

  return UpdateAction::RENDER;
}
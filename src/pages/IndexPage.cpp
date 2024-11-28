#include "pages/IndexPage.h"
#include "AbstractPage.h"
#include "Application.h"
#include "util.h"
#include <iostream>

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

UpdateAction IndexPage::Update(Application &app, std::istream &cin) {
  int inp = 0;
  cin >> inp;

  if (util::ClearFailedIstream(cin)) {
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
    app.Redirect("/add-expense");
    break;
  case view_expenses:
    app.Redirect("/view-expense");
    break;
  case search_expenses:
    app.Redirect("/search-expenses");
    break;
  case generate_report:
    app.Redirect("/generate-report");
    break;
  case save_and_exit:
    app.Redirect("/save-and-exit");
    break;
  default:
    break;
  }

  return UpdateAction::RENDER;
}
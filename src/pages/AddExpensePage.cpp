#include "pages/AddExpensePage.h"
#include "AbstractPage.h"
#include "Application.h"
#include "util.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

AddExpensePage::AddExpensePage() : date(0, 0, 0), amount(0) {};

void AddExpensePage::Render(std::ostream &cout) {
  cout << "\x1B[2J\x1B[1;1H";

  switch (state) {
  case State::PROMPT_DATE:
    cout << "Enter the date (dd/mm/yyyy): ";
    break;
  case State::PROMPT_CATEGORY:
    cout << "Enter the category (Food, Transportation, etc.): ";
    break;
  case State::PROMPT_AMOUNT:
    cout << "Enter the amount: ";
    break;
  case State::PROMPT_DESC:
    cout << "Enter a description (optional): ";
    break;
  case State::END:
  default:
    cout << "Expense added successfully!\nPress enter to return to the main "
            "menu.\n";
    break;
  }
}

UpdateAction AddExpensePage::Update(Application &app, std::istream &cin) {
  std::string inp;
  std::getline(cin, inp);
  inp = util::TrimString(inp);

  switch (state) {
  case State::PROMPT_DATE: {
    // Matches dd/mm/yyyy
    const std::regex patt(R"(^([0-9]{2})/([0-9]{2})/([0-9]{4})$)");
    std::smatch match;

    if (std::regex_match(inp, match, patt)) {
      date.day = std::stoi(match[1].str());
      date.month = std::stoi(match[2].str());
      date.year = std::stoi(match[3].str());

      if (date.IsValid()) {
        state = State::PROMPT_CATEGORY;
      }
    }
  } break;
  case State::PROMPT_CATEGORY:
    if (!inp.empty()) {
      category = inp;
      state = State::PROMPT_AMOUNT;
    }
    break;
  case State::PROMPT_AMOUNT: {
    // Matches num.num or num
    const std::regex patt(R"(^[0-9]+\.?[0-9]+$)");
    std::smatch match;

    if (std::regex_match(inp, match, patt)) {
      amount = std::stof(match.str());
      state = State::PROMPT_DESC;
    }
  } break;
  case State::PROMPT_DESC:
    desc = inp;
    state = State::END;
    break;
  case State::END:
  default: {
    std::ostringstream oss;
    oss << date.ToString() << "," << category << "," << amount << "," << desc;

    app.InsertOrAssignSharedData("new_expense", oss.str());
    app.Redirect("/");
  }
    return UpdateAction::RENDER;
  }

  return UpdateAction::RENDER;
}

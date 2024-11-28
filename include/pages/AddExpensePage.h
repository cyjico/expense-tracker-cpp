#pragma once
#include "AbstractPage.h"
#include "Date.h"
#include <cstdint>
struct Date;

class AddExpensePage : public AbstractPage {
  Date date;
  std::string category;
  float amount;
  std::string desc;

  enum class State : std::uint8_t {
    PROMPT_DATE,
    PROMPT_CATEGORY,
    PROMPT_AMOUNT,
    PROMPT_DESC,
    END,
  };

  State state = State::PROMPT_DATE;

public:
  AddExpensePage();

  void Render(std::ostream &cout) override;
  UpdateAction Update(Application &app, std::istream &cin) override;
};

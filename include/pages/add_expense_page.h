#pragma once
#include "abstract_page.h"
#include "date.h"
#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

class add_expense_page : public abstract_page {
  date m_date;
  std::string m_category;
  float m_amount;
  std::string m_desc;

  enum class state : std::uint8_t {
    prompt_date,
    prompt_category,
    prompt_amount,
    prompt_desc,
    end,
  };

  state m_state = state::prompt_date;

public:
  add_expense_page();

  void render(std::ostream &cout) override;
  update_action update(application &app, std::istream &cin) override;
};

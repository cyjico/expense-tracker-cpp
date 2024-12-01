#pragma once
#include "pages/abstract_page.h"
#include <iostream>

class search_expenses_page : public abstract_page {
public:
  search_expenses_page();

  update_action update(application &app, std::ostream &cout,
                       std::istream &cin) override;
};

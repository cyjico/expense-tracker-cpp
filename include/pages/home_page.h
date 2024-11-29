#pragma once
#include "application.h"
#include "pages/abstract_page.h"
#include <istream>
#include <ostream>

class home_page : public abstract_page {
public:
  explicit home_page(application &app);

  void render(application &app, std::ostream &cout) override;
  update_action update(application &app, std::istream &cin) override;
};

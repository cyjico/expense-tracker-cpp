#pragma once
#include "application.h"
#include "pages/abstract_page.h"
#include <istream>
#include <memory>
#include <ostream>

class home_page : public abstract_page,
                  public std::enable_shared_from_this<home_page> {
public:
  home_page();

  void attach_listeners(application &app);
  void render(application &app, std::ostream &cout) override;
  update_action update(application &app, std::istream &cin) override;
};

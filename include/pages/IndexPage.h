#pragma once
#include "AbstractPage.h"

class IndexPage : public AbstractPage {
public:
  IndexPage();

  void Render(std::ostream &cout) override;
  UpdateAction Update(Application &app, std::istream &cin) override;
};

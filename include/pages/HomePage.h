#pragma once
#include "AbstractPage.h"

class HomePage : public AbstractPage {
public:
  HomePage();

  void Render(std::ostream &cout) override;
  UpdateAction Update(Application &app, std::istream &cin) override;
};

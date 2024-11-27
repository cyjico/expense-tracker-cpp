#include "Application.h"
#include "pages/IndexPage.h"
#include <iostream>
#include <memory>

int main() {
  const PageMap pages = {{"/", std::make_shared<IndexPage>(IndexPage())}};

  Application app(pages);
  app.RunIndefinitely();

  std::cout << "Reached end of program.";
  return 0;
}

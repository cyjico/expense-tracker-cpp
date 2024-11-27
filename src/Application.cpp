#include "Application.h"
#include "AbstractPage.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

Application::Application(PageMap pages_arg) : pages(std::move(pages_arg)) {}

void Application::RunIndefinitely() {
  UpdateAction action = UpdateAction::RENDER;

  while (true) {
    auto page = this->pages.at(this->cur_address);

    switch (action) {
    case UpdateAction::NONE:
      break;
    case UpdateAction::RENDER:
      page->Render(std::cout);
      break;
    case UpdateAction::EXIT:
      return;
    }

    action = page->Update(*this, std::cin);
  }
}

void Application::Redirect(const std::string &new_address) {
  if (this->pages.find(new_address) == this->pages.end()) {
    throw std::invalid_argument(R"(Address ")" + new_address +
                                R"(" does not exist.)");
  }

  this->cur_address = new_address;
}

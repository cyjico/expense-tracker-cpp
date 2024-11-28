#include "Application.h"
#include "AbstractPage.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

Application::Application(PageMap pages) : pages(std::move(pages)) {}

void Application::RunIndefinitely() {
  UpdateAction action = UpdateAction::RENDER_NEXT_FRAME;

  while (true) {
    auto page = this->pages.at(this->cur_address);

    switch (action) {
    case UpdateAction::SKIP_RENDER_NEXT_FRAME:
      break;
    case UpdateAction::RENDER_NEXT_FRAME:
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

void Application::InsertOrAssignSharedData(const std::string &key,
                                           std::string value) {
  shared_data.insert_or_assign(key, value);
}

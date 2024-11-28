#include "application.h"
#include "observer_pattern/subject.hpp"
#include "pages/abstract_page.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

application::application(page_map pages) : m_pages(std::move(pages)) {}

void application::run_indefinitely() {
  update_action action = update_action::render_next_frame;

  while (true) {
    auto page = m_pages.at(m_cur_address);

    switch (action) {
    case update_action::skip_render_next_frame:
      break;
    case update_action::render_next_frame:
      page->render(*this, std::cout);
      break;
    case update_action::exit:
      return;
    }

    action = page->update(*this, std::cin);
  }
}

void application::redirect(const std::string &new_address) {
  if (m_pages.find(new_address) == m_pages.end()) {
    throw std::invalid_argument(R"(Address ")" + new_address +
                                R"(" does not exist.)");
  }

  m_onpageunload.notify(m_cur_address, m_pages.at(m_cur_address));

  m_cur_address = new_address;

  m_onpageload.notify(new_address, m_pages.at(new_address));
}

const std::unordered_map<std::string, std::string> &application::shared_data() {
  return m_shared_data;
}

const std::string &application::at_shared_datum(const std::string &key) {
  return m_shared_data.at(key);
}

bool application::has_shared_datum(const std::string &key) {
  return m_shared_data.find(key) != m_shared_data.end();
}

std::unordered_map<std::string, std::string>::const_iterator
application::find_shared_datum(const std::string &key) {
  return m_shared_data.find(key);
}

bool application::insert_or_assign_shared_datum(const std::string &key,
                                                std::string value) {
  return m_shared_data.insert_or_assign(key, value).second;
}

bool application::erase_shared_datum(const std::string &key) {
  return m_shared_data.erase(key) != 0U;
}

subject<std::string, const std::shared_ptr<abstract_page>> &
application::onpageload() {
  return m_onpageload;
};
subject<std::string, const std::shared_ptr<abstract_page>> &
application::onpageunload() {
  return m_onpageunload;
};

#include "application.h"
#include "events/page_event_emitter.h"
#include "pages/abstract_page.h"
#include <any>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

application::application() = default;

void application::initialize(page_map pages) { m_pages = std::move(pages); }

void application::run_indefinitely() {
  {
    page_event evt(this);
    m_onpageload.emit(evt);
  }

  update_action action = update_action::none;

  while (true) {
    auto page = m_pages.at(m_cur_address);

    switch (action) {
    case update_action::none:
      break;
    case update_action::exit_app:
      return;
    }

    action = page->update(*this, std::cout, std::cin);
  }
}

void application::redirect(const std::string &new_address) {
  if (m_pages.find(new_address) == m_pages.end()) {
    throw std::invalid_argument(R"(Address ")" + new_address +
                                R"(" does not exist.)");
  }

  page_event evt(this);
  m_onpageunload.emit(evt);

  m_cur_address = new_address;

  m_onpageload.emit(evt);
}

const std::string &application::cur_address() const { return m_cur_address; }

std::shared_ptr<const abstract_page> application::cur_page() const {
  return m_pages.at(m_cur_address);
}

const std::unordered_map<std::string, std::any> &application::shared_data() {
  return m_shared_data;
}

bool application::has_shared_datum(const std::string &key) const {
  return m_shared_data.find(key) != m_shared_data.end();
}

std::unordered_map<std::string, std::any>::const_iterator
application::find_shared_datum(const std::string &key) {
  return m_shared_data.find(key);
}

bool application::erase_shared_datum(const std::string &key) {
  return m_shared_data.erase(key) != 0U;
}

page_event_emitter &application::onpageload() { return m_onpageload; };
page_event_emitter &application::onpageunload() { return m_onpageunload; };

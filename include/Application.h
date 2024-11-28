#pragma once
#include "observer_pattern/observer.hpp"
#include "observer_pattern/subject.hpp"
#include <memory>
#include <string>
#include <unordered_map>
class abstract_page;

using page_map =
    std::unordered_map<std::string, std::shared_ptr<abstract_page>>;

namespace application_events {
using page_observer =
    observer<std::string, const std::shared_ptr<abstract_page>>;
using page_subject = subject<std::string, const std::shared_ptr<abstract_page>>;
} // namespace application_events

class application {
  page_map m_pages;
  std::string m_cur_address = "/";

  std::unordered_map<std::string, std::string> m_shared_data;
  application_events::page_subject m_onpageload;
  application_events::page_subject m_onpageunload;

public:
  application();

  void initialize(page_map pages);

  void run_indefinitely();
  void redirect(const std::string &new_address);

  const std::unordered_map<std::string, std::string> &shared_data();

  // --- Datum Accessor ---

  /**
   * @brief Retrieves a datum via key.
   *
   * @param key
   * @return
   * @throw std::out_of_range If no such datum is present.
   */
  const std::string &at_shared_datum(const std::string &key);

  // --- Datum Lookup ---

  bool has_shared_datum(const std::string &key);

  std::unordered_map<std::string, std::string>::const_iterator
  find_shared_datum(const std::string &key);

  // --- Datum Modifier ---

  /**
   * @brief Attempts to insert a datum.
   * Function initially tries to insert but, if it fails, will assign instead.
   * @param key
   * @param value
   * @return If it has successfully inserted the datum.
   */
  bool insert_or_assign_shared_datum(const std::string &key, std::string value);

  /**
   * @brief Erases a shared datum based on the key.
   *
   * @param key Key of element to be erased.
   * @return iF it has erased a present (true) or absent (false)
   * datum.
   */
  bool erase_shared_datum(const std::string &key);

  // --- Observer Pattern ---

  application_events::page_subject &onpageload();
  application_events::page_subject &onpageunload();
};

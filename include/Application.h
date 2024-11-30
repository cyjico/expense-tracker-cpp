#pragma once
#include "events/page_event_emitter.h"
#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
class abstract_page;

using page_map =
    std::unordered_map<std::string, std::shared_ptr<abstract_page>>;

class application {
  page_map m_pages;
  std::string m_cur_address = "/";

  std::unordered_map<std::string, std::any> m_shared_data;
  page_event_emitter m_onpageload;
  page_event_emitter m_onpageunload;

public:
  application();

  void initialize(page_map pages);

  void run_indefinitely();
  void redirect(const std::string &new_address);

  const std::string &cur_address() const;
  std::shared_ptr<const abstract_page> cur_page() const;

  const std::unordered_map<std::string, std::any> &shared_data();

  // --- Datum Accessor ---

  /**
   * @brief Retrieves a datum via key.
   *
   * @param key
   * @return
   * @throw std::out_of_range if no such datum is present. std::runtime_error if
   * the datum could not be casted to the requested type.
   */
  template <typename T> T &at_shared_datum(const std::string &key);
  template <typename T> const T &at_shared_datum(const std::string &key) const;

  // --- Datum Lookup ---

  bool has_shared_datum(const std::string &key) const;

  std::unordered_map<std::string, std::any>::const_iterator
  find_shared_datum(const std::string &key);

  // --- Datum Modifier ---

  /**
   * @brief Attempts to insert a datum.
   *
   * Function initially tries to insert but, if it fails, will assign instead.
   *
   * @param key
   * @param value
   * @return If it has successfully inserted the datum.
   */
  template <typename T>
  bool insert_or_assign_shared_datum(const std::string &key, const T &value);

  /**
   * @brief Erases a shared datum based on the key.
   *
   * @param key Key of element to be erased.
   * @return iF it has erased a present (true) or absent (false)
   * datum.
   */
  bool erase_shared_datum(const std::string &key);

  // --- Observer Pattern ---

  page_event_emitter &onpageload();
  page_event_emitter &onpageunload();
};

template <typename T> T &application::at_shared_datum(const std::string &key) {
  try {
    return std::any_cast<T &>(m_shared_data.at(key));
  } catch (const std::bad_any_cast &e) {
    throw std::runtime_error("Failed to cast the datum to the requested type.");
  } catch (const std::out_of_range &e) {
    throw std::runtime_error(R"(Datum with key ")" + key +
                             R"(" does not exist.)");
  }
}

template <typename T>
const T &application::at_shared_datum(const std::string &key) const {
  try {
    return std::any_cast<const T &>(m_shared_data.at(key));
  } catch (const std::bad_any_cast &e) {
    throw std::runtime_error("Failed to cast the datum to the requested type.");
  } catch (const std::out_of_range &e) {
    throw std::runtime_error(R"(Datum with key ")" + key +
                             R"(" does not exist.)");
  }
}

template <typename T>
bool application::insert_or_assign_shared_datum(const std::string &key,
                                                const T &value) {
  return m_shared_data.insert_or_assign(key, value).second;
}

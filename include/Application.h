#pragma once
#include <memory>
#include <string>
#include <unordered_map>
class abstract_page;

using page_map =
    std::unordered_map<std::string, std::shared_ptr<abstract_page>>;

class application {
  page_map m_pages;
  std::string m_cur_address = "/";

  std::unordered_map<std::string, std::string> m_shared_data;

public:
  explicit application(page_map pages);

  void run_indefinitely();
  void redirect(const std::string &new_address);

  const std::unordered_map<const std::string, std::string> &get_shared_data();
  void insert_or_assign_shared_data(const std::string &key, std::string value);
  bool erase_shared_data(const std::string &key);
  const std::string &at_shared_data(const std::string &key);
};

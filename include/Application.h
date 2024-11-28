#pragma once
#include <memory>
#include <string>
#include <unordered_map>
class AbstractPage;

using PageMap = std::unordered_map<std::string, std::shared_ptr<AbstractPage>>;

class Application {
  PageMap pages;
  std::string cur_address = "/";

  std::unordered_map<std::string, std::string> shared_data;

public:
  explicit Application(PageMap pages);

  void RunIndefinitely();
  void Redirect(const std::string &new_address);

  const std::unordered_map<const std::string, std::string> &GetSharedData();
  void InsertOrAssignSharedData(const std::string &key, std::string value);
  bool EraseSharedData(const std::string &key);
  const std::string &AtSharedData(const std::string &key);
};

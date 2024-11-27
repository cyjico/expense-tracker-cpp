#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class AbstractPage;

using PageMap = std::unordered_map<std::string, std::shared_ptr<AbstractPage>>;

class Application {
  PageMap pages;
  std::string cur_address = "/";

public:
  explicit Application(PageMap pages_arg);

  void RunIndefinitely();
  void Redirect(const std::string &new_address);
};

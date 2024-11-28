#pragma once
#include "observer.hpp"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

template <typename... Args> class subject {
  using observer_ptr = std::shared_ptr<observer<Args...>>;

  std::vector<observer_ptr> m_observers;

public:
  void register_observer(observer_ptr observer) {
    m_observers.push_back(observer);
  };
  void remove_observer(observer_ptr observer) {
    m_observers.erase(
        std::remove(m_observers.begin(), m_observers.end(), observer),
        m_observers.end());
  }

  void notify(Args... args) {
    for (std::size_t i = 0; i != m_observers.size(); ++i) {
      m_observers.at(i)->update(args...);
    }
  }
};
#pragma once
#include <algorithm>
#include <functional>
#include <vector>

template <typename T> class event_emitter {
public:
  using listener = std::function<void(T &event)>;

private:
  std::vector<listener> m_listeners;

public:
  void add_listener(listener listener) { m_listeners.push_back(listener); }

  void remove_listener(listener listener) {
    auto itr = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (itr != m_listeners.end()) {
      m_listeners.erase(itr);
    }
  }

  void emit(T &event) {
    std::vector<listener> listeners_copy = m_listeners;

    for (auto &listener : listeners_copy) {
      listener(event);
    }
  }
};

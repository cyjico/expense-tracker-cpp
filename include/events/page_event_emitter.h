#pragma once
#include "event_emitter.hpp"
class application;

struct page_event {
  application *app;

  explicit page_event(application *param_app);
};

using page_event_emitter = event_emitter<page_event>;

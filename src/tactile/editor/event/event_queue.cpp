// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/editor/event/event_queue.hpp"

namespace tactile::editor {

EventQueue::EventQueue()
{
  m_pools.reserve(64uz);
}

void EventQueue::clear()
{
  m_pending.clear();
  for (const auto& [event_id, pool] : m_pools) {
    pool->clear_events();
  }
}

void EventQueue::publish()
{
  while (!m_pending.empty()) {
    const auto event_id = m_pending.front();
    m_pending.pop_front();
    m_pools.at(event_id)->publish_next_event();
  }
}

auto EventQueue::size() const -> usize
{
  return m_pending.size();
}

}  // namespace tactile::editor

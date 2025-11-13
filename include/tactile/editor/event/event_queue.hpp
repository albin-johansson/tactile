// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <cassert>
#include <concepts>
#include <utility>

#include "tactile/core/container/deque.hpp"
#include "tactile/core/container/function.hpp"
#include "tactile/core/container/hash_map.hpp"
#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/macros.hpp"
#include "tactile/core/primitives.hpp"
#include "tactile/editor/event/event_id.hpp"

namespace tactile::editor {

template <typename T>
concept EventType = requires {
  { T::kID } -> std::convertible_to<EventID>;
};

namespace event_queue {

/// Interface used to type-erase event pool implementations.
class IEventPool
{
 public:
  TACTILE_INTERFACE(IEventPool);

  /// Removes all pending events from the pool.
  virtual void clear_events() = 0;

  /// Publishes (and removes) the next pending event.
  ///
  /// \pre There must be a pending event.
  virtual void publish_next_event() = 0;
};

/// Represents an event pool for the event type T.
template <EventType T>
class EventPool final : public IEventPool
{
 public:
  using callback_type = Function<void(const T&)>;

  void clear_events() override { m_events.clear(); }

  void publish_next_event() override
  {
    assert(!m_events.empty());
    if (m_callback) {
      m_callback(m_events.front());
    }
    m_events.pop_front();
  }

  void connect(callback_type&& callback) { m_callback = std::move(callback); }

  void disconnect() { m_callback = callback_type {}; }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  void enqueue(Args&&... args)
  {
    m_events.emplace_back(std::forward<Args>(args)...);
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  void trigger(Args&&... args)
  {
    if (m_callback) {
      m_callback(T {std::forward<Args>(args)...});
    }
  }

 private:
  Deque<T> m_events {};
  callback_type m_callback {};
};

}  // namespace event_queue

/// Represents a queue of heterogeneous events.
class EventQueue final
{
 public:
  EventQueue();

  /// Removes all pending events from the queue without publishing them.
  void clear();

  /// Publishes (and removes) all pending events in the submitted order.
  void publish();

  /// Returns the number of pending events in the queue.
  auto size() const -> usize;

  /// Installs a callback that will be called whenever an event of type T is published.
  template <EventType T, std::invocable<const T&> Callback>
  void connect(Callback callback)
  {
    auto& pool = get_or_create_pool<T>();
    pool.connect(std::move(callback));
  }

  /// Uninstalls any previously installed callback for the event type T.
  template <EventType T>
  void disconnect()
  {
    auto& pool = get_or_create_pool<T>();
    pool.disconnect();
  }

  /// Adds an event to the end of the queue.
  template <EventType T, typename... Args>
    requires std::constructible_from<T, Args...>
  void enqueue(Args&&... args)
  {
    auto& pool = get_or_create_pool<T>();
    pool.enqueue(std::forward<Args>(args)...);

    m_pending.push_back(T::kID);
  }

  /// Immediately publishes an event, bypassing any pending events.
  template <EventType T, typename... Args>
    requires std::constructible_from<T, Args...>
  void trigger(Args&&... args)
  {
    auto& pool = get_or_create_pool<T>();
    pool.trigger(std::forward<Args>(args)...);
  }

 private:
  HashMap<EventID, Unique<event_queue::IEventPool>> m_pools {};
  Deque<EventID> m_pending {};

  template <EventType T>
  auto get_or_create_pool() -> event_queue::EventPool<T>&
  {
    if (const auto iter = m_pools.find(T::kID); iter != m_pools.end()) {
      return dynamic_cast<event_queue::EventPool<T>&>(*iter->second);
    }

    auto pool = make_unique<event_queue::EventPool<T>>();
    auto* const pool_ptr = pool.get();

    (void) m_pools.insert_or_assign(T::kID, std::move(pool));

    return *pool_ptr;
  }
};

}  // namespace tactile::editor

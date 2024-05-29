// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // forward

#include <entt/signal/dispatcher.hpp>

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Provides the event handling API.
 */
class EventDispatcher final
{
 public:
  /**
   * Flushes all pending events.
   */
  void update();

  /**
   * Pushes an event to the event queue.
   *
   * \details
   * Use the \c update function to issue enqueued events.
   *
   * \tparam Event The event type.
   * \tparam Args  The event argument types.
   *
   * \param args The arguments used to construct the event.
   */
  template <typename Event, typename... Args>
  void push(Args&&... args)
  {
    if (!mDispatcher.sink<Event>().empty()) {
      mDispatcher.enqueue<Event>(std::forward<Args>(args)...);
    }
  }

  /**
   * Immediately triggers the registered listeners for a given event.
   *
   * \tparam Event The event type.
   * \tparam Args  The event argument types.
   *
   * \param args The arguments used to construct the event.
   */
  template <typename Event, typename... Args>
  void trigger(Args&&... args)
  {
    if (!mDispatcher.sink<Event>().empty()) {
      mDispatcher.trigger(Event {std::forward<Args>(args)...});
    }
  }

  /**
   * Binds a handler to an event.
   *
   * \tparam Event The event type.
   * \tparam Slot  The slot signature.
   * \tparam Args  The slot argument types.
   *
   * \param args The slot arguments.
   */
  template <typename Event, auto Slot, typename... Args>
  void bind(Args&&... args)
  {
    mDispatcher.sink<Event>().template connect<Slot>(
        std::forward<Args>(args)...);
  }

 private:
  entt::dispatcher mDispatcher {};
};

}  // namespace tactile

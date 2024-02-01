// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <tuple>  // tuple, apply, get

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/ui/widget.hpp"
#include "tactile/foundation/misc/concepts.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class Model;

/**
 * Manages a collection of widget-like objects.
 *
 * \note
 *    The order of the type parameters is significant, since widgets are updated
 *    in the order that their types are specified in the type parameter list.
 *
 * \tparam Widgets The widget types to store.
 */
template <Widget... Widgets>
class WidgetManager final {
 public:
  /**
   * Updates all widgets in order.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher to use.
   */
  void update(const Model& model, EventDispatcher& dispatcher)
  {
    std::apply([&](auto&&... widgets) { (widgets.update(model, dispatcher), ...); },
               mWidgets);
  }

  /**
   * Returns a widget from the collection.
   *
   * \tparam T The type of the desired widget.
   *
   * \return
   *    A widget.
   */
  template <OneOf<Widgets...> T>
  [[nodiscard]] auto get() -> T&
  {
    return std::get<T>(mWidgets);
  }

 private:
  std::tuple<Widgets...> mWidgets {};
};

}  // namespace tactile::core

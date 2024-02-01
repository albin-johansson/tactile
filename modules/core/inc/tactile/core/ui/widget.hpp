// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // default_initializable, same_as

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class Model;

// clang-format off

/**
 * Describes widget types.
 *
 * \tparam T An arbitrary type.
 */
template <typename T>
concept WidgetType = std::default_initializable<T> &&
                     requires(T widget, const Model& model, EventDispatcher& dispatcher) {
  { widget.update(model, dispatcher) } -> std::same_as<void>;
};

/**
 * Describes dialog types.
 *
 * \tparam T An arbitrary type.
 */
template <typename T>
concept DialogType = WidgetType<T> && requires(T dialog) {
  { dialog.open() };
};

// clang-format on

}  // namespace tactile::core

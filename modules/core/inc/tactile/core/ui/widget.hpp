// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // default_initializable, same_as

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class Model;

// clang-format off

/**
 * Describes widget types.
 *
 * \tparam T An arbitrary type.
 */
template <typename T>
concept Widget = std::default_initializable<T> &&
                 requires(T widget, const Model& model, EventDispatcher& dispatcher) {
  { widget.update(model, dispatcher) } -> std::same_as<void>;
};

// clang-format on

}  // namespace tactile

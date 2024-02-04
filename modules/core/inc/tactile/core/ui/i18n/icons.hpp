// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * Provides identifiers for icon strings.
 */
enum class IconID : int8 {
  kPlus,
  kMinus,
  kClone,
  kUpArrow,
  kDownArrow
};

/**
 * Injects the appropriate icons to a collection of translated strings.
 *
 * \pre The size of the vector must be equal to `StringID::kMAX`.
 *
 * \param strings The collection of translated strings.
 */
TACTILE_CORE_API void inject_icons(Vector<String>& strings);

/**
 * Returns a language-independent icon string.
 *
 * \note
 *    This function should only be used for standalone icons,
 *    translated strings already have icons added to them.
 *
 * \param icon_id The identifier associated with the desired icon.
 *
 * \return
 *    An icon string.
 */
[[nodiscard]]
TACTILE_CORE_API auto get_icon_string(IconID icon_id) -> const char*;

}  // namespace tactile::core

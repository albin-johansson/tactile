// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::ui {

/**
 * \brief Displays a tooltip after the last item has been hovered for a while.
 *
 * \param id   the tooltip identifier.
 * \param text the tooltip text.
 */
TACTILE_CORE_API void lazy_tooltip(const char* id, const char* text);

}  // namespace tactile::ui

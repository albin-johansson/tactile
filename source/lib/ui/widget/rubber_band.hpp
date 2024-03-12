// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/region.hpp"
#include "tactile/base/container/maybe.hpp"

struct ImVec2;

namespace tactile::ui {

/// Updates a "rubber band" (rectangular selection) in the current canvas.
///
/// \param scroll_offset the current viewport scroll offset.
/// \param tile_size the logical tile size.
///
/// \return the current selection; an empty optional if there is none.
[[nodiscard]] auto ui_rubber_band(const ImVec2& scroll_offset, const ImVec2& tile_size)
    -> Maybe<Region>;

}  // namespace tactile::ui

#pragma once

#include "common/maybe.hpp"
#include "core/region.hpp"

struct ImVec2;

namespace Tactile {

/**
 * \brief Updates a "rubber band" (rectangular selection) in the current canvas.
 *
 * \ingroup gui
 *
 * \param offset the current viewport scroll offset.
 * \param tileSize the logical tile size.
 *
 * \return the current selection; `nothing` if there is none.
 */
[[nodiscard]] auto RubberBand(const ImVec2& offset, const ImVec2& tileSize)
    -> Maybe<Region>;

}  // namespace Tactile

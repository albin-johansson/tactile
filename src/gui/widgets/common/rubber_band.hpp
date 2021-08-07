#pragma once

#include "aliases/maybe.hpp"
#include "core/tileset/tileset_selection.hpp"

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
 * \todo Consider adding general `MapRegion` struct that replaces `TilesetSelection`
 * and `RenderBounds`.
 *
 * \return the current selection; `nothing` if there is none.
 */
[[nodiscard]] auto RubberBand(const ImVec2& offset, const ImVec2& tileSize)
    -> Maybe<TilesetSelection>;

}  // namespace Tactile

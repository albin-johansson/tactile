#pragma once

#include <imgui.h>

namespace Tactile {

class Tileset;

/**
 * \brief Returns the UV tile size for a tileset.
 *
 * \ingroup rendering
 *
 * \details Use this function to acquire a vector that can be used to render
 * individual tiles from a tileset image.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the UV tile size for rendering individual tiles.
 */
[[nodiscard]] auto GetTileSizeUV(const Tileset& tileset) -> ImVec2;

}  // namespace Tactile

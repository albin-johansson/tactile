#pragma once

struct ImVec2;

namespace Tactile {

class TileLayer;
class TilesetManager;
struct RenderBounds;

void RenderTileLayer(const TileLayer& layer,
                     const TilesetManager& tilesets,
                     const ImVec2& mapPos,
                     const ImVec2& gridSize,
                     const RenderBounds& bounds);

}  // namespace Tactile

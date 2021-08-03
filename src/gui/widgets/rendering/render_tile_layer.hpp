#pragma once

namespace Tactile {

class TileLayer;
class TilesetManager;
struct RenderInfo;

void RenderTileLayer(const TileLayer& layer,
                     const TilesetManager& tilesets,
                     const RenderInfo& info);

}  // namespace Tactile

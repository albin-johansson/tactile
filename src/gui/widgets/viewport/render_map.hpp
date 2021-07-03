#pragma once

struct ImVec2;

namespace Tactile {

class MapDocument;

void RenderMap(const MapDocument& document,
               const ImVec2& mapPos,
               const ImVec2& tileSize);

}  // namespace Tactile

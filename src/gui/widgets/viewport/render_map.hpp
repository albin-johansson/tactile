#pragma once

struct ImDrawList;
struct ImVec2;

namespace Tactile {

class MapDocument;

void RenderMap(const MapDocument& document,
               ImDrawList* drawList,
               ImVec2 mapPos,
               ImVec2 tileSize);

}  // namespace Tactile

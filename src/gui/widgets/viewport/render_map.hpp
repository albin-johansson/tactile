#pragma once

struct ImDrawList;
struct ImVec2;

namespace tactile {

class MapDocument;

void RenderMap(const MapDocument& document,
               ImDrawList* drawList,
               ImVec2 offset,
               ImVec2 tileSize);

}  // namespace tactile

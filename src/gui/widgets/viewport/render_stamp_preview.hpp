#pragma once

struct ImDrawList;
struct ImVec2;

namespace Tactile {

class Map;
class MapPosition;
class Tileset;

void RenderStampPreview(ImDrawList* drawList,
                        const ImVec2& mapPos,
                        const ImVec2& gridSize,
                        const Map& map,
                        const Tileset& tileset,
                        const MapPosition& mousePos);

}  // namespace Tactile

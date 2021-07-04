#pragma once

struct ImVec2;

namespace Tactile {

struct CanvasInfo;
class MapDocument;

void RenderMap(const MapDocument& document,
               const CanvasInfo& canvas,
               const ImVec2& mapPos,
               const ImVec2& tileSize);

}  // namespace Tactile

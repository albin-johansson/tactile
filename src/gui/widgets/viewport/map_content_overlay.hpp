#pragma once

namespace Tactile {

class MapDocument;

struct CanvasInfo;
struct ViewportCursorInfo;

void MapContentOverlay(const MapDocument& document,
                       const CanvasInfo& canvas,
                       const ViewportCursorInfo& cursor);

}  // namespace Tactile

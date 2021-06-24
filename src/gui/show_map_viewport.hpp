#pragma once

namespace tactile {

class MapDocument;

void ShowMapViewport(const MapDocument* document);

void SetMapViewportGridEnabled(bool enabled) noexcept;

}  // namespace tactile

#pragma once

namespace tactile {

class Model;

void ShowMapViewport(const Model& model);

void CenterMapViewport();

void SetMapViewportGridEnabled(bool enabled) noexcept;

}  // namespace tactile

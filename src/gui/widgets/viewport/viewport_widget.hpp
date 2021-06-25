#pragma once

namespace tactile {

class Model;

void UpdateViewportWidget(const Model& model);

void CenterViewport();

void SetViewportGridEnabled(bool enabled) noexcept;

}  // namespace tactile

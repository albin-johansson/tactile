#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Model;

void UpdateViewportWidget(const Model& model, entt::dispatcher& dispatcher);

void CenterViewport();

void SetViewportGridEnabled(bool enabled) noexcept;

}  // namespace tactile

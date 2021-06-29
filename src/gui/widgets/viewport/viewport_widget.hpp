#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Model;

void UpdateViewportWidget(const Model& model, entt::dispatcher& dispatcher);

void CenterViewport();

}  // namespace tactile

#pragma once

#include <entt.hpp> // dispatcher

namespace tactile {

class Model;

void UpdateLayerWidget(const Model& model, entt::dispatcher& dispatcher);

void SetLayerWidgetVisible(bool visible) noexcept;

[[nodiscard]] auto IsLayerWidgetVisible() noexcept -> bool;

}  // namespace tactile

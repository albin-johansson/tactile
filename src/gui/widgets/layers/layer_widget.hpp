#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateLayerWidget(const Model& model, entt::dispatcher& dispatcher);

void SetLayerWidgetVisible(bool visible) noexcept;

[[nodiscard]] auto IsLayerWidgetVisible() noexcept -> bool;

}  // namespace Tactile

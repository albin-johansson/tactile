#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Model;

void UpdatePropertiesWidget(const Model& model, entt::dispatcher& dispatcher);

void SetPropertiesWidgetVisible(bool visible) noexcept;

[[nodiscard]] auto IsPropertiesWidgetVisible() noexcept -> bool;

}  // namespace tactile

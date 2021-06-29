#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdatePropertiesWidget(const Model& model, entt::dispatcher& dispatcher);

void SetPropertiesWidgetVisible(bool visible) noexcept;

[[nodiscard]] auto IsPropertiesWidgetVisible() noexcept -> bool;

}  // namespace Tactile

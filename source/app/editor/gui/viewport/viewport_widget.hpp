#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace tactile {

class Model;
class icon_manager;

void UpdateViewportWidget(const Model& model,
                          const icon_manager& icons,
                          entt::dispatcher& dispatcher);

void CenterViewport();

[[nodiscard]] auto IsViewportFocused() noexcept -> bool;

[[nodiscard]] auto IsMouseWithinViewport() noexcept -> bool;

}  // namespace tactile

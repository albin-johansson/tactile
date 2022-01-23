#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace tactile {

class Model;
class Icons;

void UpdateViewportWidget(const Model& model,
                          const Icons& icons,
                          entt::dispatcher& dispatcher);

void CenterViewport();

[[nodiscard]] auto IsViewportFocused() noexcept -> bool;

[[nodiscard]] auto IsMouseWithinViewport() noexcept -> bool;

}  // namespace tactile

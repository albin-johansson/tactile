#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;
class Icons;

void UpdateViewportWidget(const Model& model,
                          const Icons& icons,
                          entt::dispatcher& dispatcher);

void CenterViewport();

[[nodiscard]] auto IsViewportFocused() noexcept -> bool;

}  // namespace Tactile

#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateViewportWidget(const Model& model, entt::dispatcher& dispatcher);

void CenterViewport();

[[nodiscard]] auto IsViewportFocused() noexcept -> bool;

}  // namespace Tactile

#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace tactile {

class document_model;
class icon_manager;

void UpdateViewportWidget(const document_model& model,
                          const icon_manager& icons,
                          entt::dispatcher& dispatcher);

void CenterViewport();

[[nodiscard]] auto IsViewportFocused() noexcept -> bool;

[[nodiscard]] auto IsMouseWithinViewport() noexcept -> bool;

}  // namespace tactile

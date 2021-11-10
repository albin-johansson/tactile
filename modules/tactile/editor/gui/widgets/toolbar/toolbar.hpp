#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateToolbarWidget(const Model& model, entt::dispatcher& dispatcher);

void SetToolbarVisible(bool visible) noexcept;

[[nodiscard]] auto IsToolbarVisible() noexcept -> bool;

[[nodiscard]] auto IsToolbarFocused() noexcept -> bool;

}  // namespace Tactile

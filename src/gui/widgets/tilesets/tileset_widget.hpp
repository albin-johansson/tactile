#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateTilesetWidget(const Model& model, entt::dispatcher& dispatcher);

void SetTilesetWidgetVisible(bool visible) noexcept;

[[nodiscard]] auto IsTilesetWidgetVisible() noexcept -> bool;

}  // namespace Tactile

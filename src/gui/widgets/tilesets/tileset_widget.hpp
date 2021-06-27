#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Model;

void UpdateTilesetWidget(const Model& model, entt::dispatcher& dispatcher);

void SetTilesetWidgetVisible(bool visible) noexcept;

[[nodiscard]] auto IsTilesetWidgetVisible() noexcept -> bool;

}  // namespace tactile

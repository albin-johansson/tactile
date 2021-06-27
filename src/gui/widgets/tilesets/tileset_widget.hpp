#pragma once

namespace tactile {

void UpdateTilesetWidget();

void SetTilesetWidgetVisible(bool visible) noexcept;

[[nodiscard]] auto IsTilesetWidgetVisible() noexcept -> bool;

}  // namespace tactile

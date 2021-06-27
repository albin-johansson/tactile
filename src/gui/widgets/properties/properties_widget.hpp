#pragma once

namespace tactile {

void UpdatePropertiesWidget();

void SetPropertiesWidgetVisible(bool visible) noexcept;

[[nodiscard]] auto IsPropertiesWidgetVisible() noexcept -> bool;

}  // namespace tactile

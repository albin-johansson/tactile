#pragma once

#include <string>  // string

#include "aliases/ints.hpp"
#include "gui/themes.hpp"

namespace Tactile {

// Loads existing persistent preferences or loads the default preferences
void LoadPreferences();

// Saves the current preferences in the persistent file directory
void SavePreferences();

namespace Prefs {

void SetPreferredFormat(std::string format);

void SetShowGrid(bool show) noexcept;

void SetEmbedTilesets(bool embed) noexcept;

void SetHumanReadableOutput(bool readable) noexcept;

void SetShowLayerDock(bool visible) noexcept;

void SetShowTilesetDock(bool visible) noexcept;

void SetShowPropertiesDock(bool visible) noexcept;

void SetTheme(Theme theme) noexcept;

void SetWindowBorder(bool enabled) noexcept;

void SetRestoreLayout(bool restore) noexcept;

void SetCommandCapacity(usize capacity) noexcept;

[[nodiscard]] auto GetPreferredFormat() -> const std::string&;

[[nodiscard]] auto GetShowGrid() noexcept -> bool;

[[nodiscard]] auto GetEmbedTilesets() noexcept -> bool;

[[nodiscard]] auto GetHumanReadableOutput() noexcept -> bool;

[[nodiscard]] auto GetShowLayerDock() noexcept -> bool;

[[nodiscard]] auto GetShowTilesetDock() noexcept -> bool;

[[nodiscard]] auto GetShowPropertiesDock() noexcept -> bool;

[[nodiscard]] auto GetTheme() noexcept -> Theme;

[[nodiscard]] auto GetWindowBorder() noexcept -> bool;

[[nodiscard]] auto GetRestoreLayout() noexcept -> bool;

[[nodiscard]] auto GetCommandCapacity() noexcept -> usize;

}  // namespace Prefs
}  // namespace Tactile

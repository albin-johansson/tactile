#pragma once

#include <string>  // string

#include "aliases/ints.hpp"
#include "gui/themes.hpp"

namespace Tactile {

struct Preferences final
{
  std::string preferred_format;
  Theme theme;
  usize command_capacity;
  bool embed_tilesets;
  bool human_readable_output;
  bool show_grid;
  bool show_layer_dock;
  bool show_tileset_dock;
  bool show_properties_dock;
  bool window_border;
  bool restore_layout;
};

/// Loads existing persistent preferences or loads the default preferences.
void LoadPreferences();

/// Saves the current preferences in the persistent file directory.
void SavePreferences();

/// Overrides the current preferences.
void SetPreferences(Preferences prefs);

/// Returns a snapshot of the current preferences.
[[nodiscard]] auto GetPreferences() -> Preferences;

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

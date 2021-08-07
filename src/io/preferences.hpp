#pragma once

#include <string>  // string

#include "aliases/ints.hpp"
#include "gui/themes.hpp"

namespace Tactile {

/// \addtogroup io
/// \{

/// \name Preference API
/// \{

struct Preferences final
{
  std::string preferred_format;
  std::string theme;
  usize command_capacity;
  int preferred_tile_width;
  int preferred_tile_height;
  bool embed_tilesets;
  bool human_readable_output;
  bool show_grid;
  bool show_layer_dock;
  bool show_tileset_dock;
  bool show_properties_dock;
  bool window_border;
  bool restore_layout;
  bool restore_last_session;
};

/// Loads existing persistent preferences or loads the default preferences.
void LoadPreferences();

/// Saves the current preferences in the persistent file directory.
void SavePreferences();

/// Overrides the current preferences.
void SetPreferences(Preferences prefs);

/// Returns a snapshot of the current preferences.
[[nodiscard]] auto GetPreferences() -> const Preferences&;

/// \} End of preference API

/// \} End of group io

namespace Prefs {

/// \addtogroup io
/// \{

/// \name Preference API
/// \{

/// \name Appearance
/// \{

void ResetAppearancePreferences(Preferences& prefs);
void SetShowGrid(bool show) noexcept;

[[nodiscard]] auto GetTheme() noexcept -> const std::string&;
[[nodiscard]] auto GetWindowBorder() noexcept -> bool;
[[nodiscard]] auto GetShowGrid() noexcept -> bool;

/// \} End of appearance

/// \name Behavior
/// \{

void ResetBehaviorPreferences(Preferences& prefs);

[[nodiscard]] auto GetCommandCapacity() noexcept -> usize;
[[nodiscard]] auto GetPreferredTileWidth() noexcept -> int;
[[nodiscard]] auto GetPreferredTileHeight() noexcept -> int;
[[nodiscard]] auto GetRestoreLastSession() noexcept -> bool;

/// \} End of behavior

/// \name Export
/// \{

void ResetExportPreferences(Preferences& prefs);

[[nodiscard]] auto GetPreferredFormat() -> const std::string&;
[[nodiscard]] auto GetEmbedTilesets() noexcept -> bool;
[[nodiscard]] auto GetHumanReadableOutput() noexcept -> bool;

/// \} End of export

/// \name Widgets
/// \{

void SetShowLayerDock(bool visible) noexcept;
void SetShowTilesetDock(bool visible) noexcept;
void SetShowPropertiesDock(bool visible) noexcept;

[[nodiscard]] auto GetShowLayerDock() noexcept -> bool;
[[nodiscard]] auto GetShowTilesetDock() noexcept -> bool;
[[nodiscard]] auto GetShowPropertiesDock() noexcept -> bool;
[[nodiscard]] auto GetRestoreLayout() noexcept -> bool;

/// \} End of widgets

/// \} End of preference API

/// \} End of group io

}  // namespace Prefs
}  // namespace Tactile

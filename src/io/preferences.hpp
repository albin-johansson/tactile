#pragma once

#include <centurion.hpp>  // color
#include <string>         // string

#include "common/ints.hpp"

namespace Tactile {

/// \addtogroup io
/// \{
/// \name Preference API
/// \{

/* Do not change the values of this enum! */
enum class OverlayPos
{
  TopLeft = 0,
  TopRight = 1,
  BottomLeft = 2,
  BottomRight = 3
};

/// \brief Represents the different available themes.
enum class Theme
{
  DearDark = 0,   ///< The standard Dear ImGui dark theme.
  DearLight = 1,  ///< The standard Dear ImGui light theme.
  Ruby = 2,
  Sapphire = 3,
  Emerald = 4,
  Amethyst = 5,
  Amber = 6,
  Nocturnal = 7,
  Ash = 8
};

struct Preferences final
{
  std::string preferred_format;
  Theme theme;
  cen::color viewport_background;
  usize command_capacity;
  int preferred_tile_width;
  int preferred_tile_height;
  int viewport_overlay_pos;
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

void ResetAppearancePreferences(Preferences& prefs);
void SetShowGrid(bool show) noexcept;

[[nodiscard]] auto GetTheme() noexcept -> Theme;
[[nodiscard]] auto GetViewportBackground() noexcept -> const cen::color&;
[[nodiscard]] auto GetWindowBorder() noexcept -> bool;
[[nodiscard]] auto GetShowGrid() noexcept -> bool;

void ResetBehaviorPreferences(Preferences& prefs);

[[nodiscard]] auto GetCommandCapacity() noexcept -> usize;
[[nodiscard]] auto GetPreferredTileWidth() noexcept -> int;
[[nodiscard]] auto GetPreferredTileHeight() noexcept -> int;
[[nodiscard]] auto GetRestoreLastSession() noexcept -> bool;

void ResetExportPreferences(Preferences& prefs);

[[nodiscard]] auto GetPreferredFormat() -> const std::string&;
[[nodiscard]] auto GetEmbedTilesets() noexcept -> bool;
[[nodiscard]] auto GetHumanReadableOutput() noexcept -> bool;

void SetShowLayerDock(bool visible) noexcept;
void SetShowTilesetDock(bool visible) noexcept;
void SetShowPropertiesDock(bool visible) noexcept;
void SetViewportOverlayPos(OverlayPos pos) noexcept;

[[nodiscard]] auto GetShowLayerDock() noexcept -> bool;
[[nodiscard]] auto GetShowTilesetDock() noexcept -> bool;
[[nodiscard]] auto GetShowPropertiesDock() noexcept -> bool;
[[nodiscard]] auto GetRestoreLayout() noexcept -> bool;
[[nodiscard]] auto GetViewportOverlayPos() noexcept -> OverlayPos;

/// \} End of preference API
/// \} End of group io

}  // namespace Prefs
}  // namespace Tactile

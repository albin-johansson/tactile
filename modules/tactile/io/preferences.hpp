#pragma once

#include <string>  // string

#include <centurion.hpp>  // color
#include <tactile_def.hpp>

namespace tactile {

/// \addtogroup io
/// \{
/// \name Preference API
/// \{

/* Do not change the values of this enum! */
enum class OverlayPos {
  TopLeft = 0,
  TopRight = 1,
  BottomLeft = 2,
  BottomRight = 3
};

/// \brief Represents the different available themes.
enum class Theme {
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
  inline static constexpr uint64 embed_tilesets = 1u << 0u;
  inline static constexpr uint64 indent_output = 1u << 1u;
  inline static constexpr uint64 fold_tile_data = 1u << 2u;
  inline static constexpr uint64 show_grid = 1u << 3u;
  inline static constexpr uint64 show_layer_dock = 1u << 4u;
  inline static constexpr uint64 show_tileset_dock = 1u << 5u;
  inline static constexpr uint64 show_properties_dock = 1u << 6u;
  inline static constexpr uint64 show_log_dock = 1u << 7u;
  inline static constexpr uint64 window_border = 1u << 8u;
  inline static constexpr uint64 restore_layout = 1u << 9u;
  inline static constexpr uint64 restore_last_session = 1u << 10u;
  inline static constexpr uint64 show_component_dock = 1u << 11u;

  std::string preferred_format;
  Theme theme;
  cen::color viewport_background;
  usize command_capacity;
  int preferred_tile_width;
  int preferred_tile_height;
  int viewport_overlay_pos;
  uint64 flags;

  void SetFlag(uint64 flag, bool value) noexcept;

  void ResetFlag(uint64 flag) noexcept;
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
[[nodiscard]] auto GetFoldTileData() noexcept -> bool;
[[nodiscard]] auto GetIndentOutput() noexcept -> bool;

void ResetDockVisibilities() noexcept;
void SetShowLayerDock(bool visible) noexcept;
void SetShowTilesetDock(bool visible) noexcept;
void SetShowPropertiesDock(bool visible) noexcept;
void SetShowLogDock(bool visible) noexcept;
void SetShowComponentDock(bool visible) noexcept;
void SetViewportOverlayPos(OverlayPos pos) noexcept;

[[nodiscard]] auto GetShowLayerDock() noexcept -> bool;
[[nodiscard]] auto GetShowTilesetDock() noexcept -> bool;
[[nodiscard]] auto GetShowPropertiesDock() noexcept -> bool;
[[nodiscard]] auto GetShowLogDock() noexcept -> bool;
[[nodiscard]] auto GetShowComponentDock() noexcept -> bool;
[[nodiscard]] auto GetRestoreLayout() noexcept -> bool;
[[nodiscard]] auto GetViewportOverlayPos() noexcept -> OverlayPos;

/// \} End of preference API
/// \} End of group io

}  // namespace Prefs
}  // namespace tactile

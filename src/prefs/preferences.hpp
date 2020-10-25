#pragma once

#include <QByteArray>
#include <QPalette>
#include <QString>

#include "setting.hpp"

/**
 * @namespace tactile::prefs
 *
 * @brief Contains entities related to persistent settings (a.k.a. preferences).
 *
 * @since 0.1.0
 */
namespace tactile::prefs {

/**
 * @namespace tactile::prefs::window
 *
 * @brief Contains window-related settings.
 *
 * @since 0.1.0
 */
namespace window {

/**
 * @brief Returns a setting that represents the last layout geometry of the
 * main window.
 *
 * @return a setting for the last layout geometry of the main window.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto last_layout_geometry() -> setting<QByteArray>;

/**
 * @brief Returns a setting that represents the last layout state of the
 * main window.
 *
 * @return a setting for the last layout state of the main window.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto last_layout_state() -> setting<QByteArray>;

}  // namespace window

/**
 * @namespace tactile::prefs::graphics
 *
 * @brief Contains graphics-related settings.
 *
 * @since 0.1.0
 */
namespace graphics {

/**
 * @brief Returns a setting that indicates whether or not a grid should be
 * rendered on maps.
 *
 * @return a boolean setting for rendering grids on maps.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto render_grid() -> setting<bool>;

[[nodiscard]] constexpr auto render_grid_def() noexcept -> bool
{
  return false;
}

/**
 * @brief Returns a setting that represents the theme of the application.
 *
 * @return a setting that represents the theme of the application.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto theme() -> setting<QPalette>;

/**
 * @brief Returns a setting that represents the name of the application theme.
 *
 * @return a setting that represents the name of the application theme.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto theme_name() -> setting<QString>;

[[nodiscard]] auto theme_name_def() -> const QString&;

[[nodiscard]] auto tool_widget_visible() -> setting<bool>;

[[nodiscard]] auto layer_widget_visible() -> setting<bool>;

[[nodiscard]] auto tileset_widget_visible() -> setting<bool>;

}  // namespace graphics

namespace saves {

[[nodiscard]] auto embed_tilesets() -> setting<bool>;

[[nodiscard]] constexpr auto embed_tilesets_def() noexcept -> bool
{
  return false;
}

[[nodiscard]] auto generate_defaults() -> setting<bool>;

[[nodiscard]] constexpr auto generate_defaults_def() noexcept -> bool
{
  return false;
}

[[nodiscard]] auto readable_output() -> setting<bool>;

[[nodiscard]] constexpr auto readable_output_def() noexcept -> bool
{
  return true;
}

[[nodiscard]] auto tile_width() -> setting<int>;

[[nodiscard]] constexpr auto tile_width_def() noexcept -> int
{
  return 32;
}

[[nodiscard]] auto tile_height() -> setting<int>;

[[nodiscard]] constexpr auto tile_height_def() noexcept -> int
{
  return 32;
}

[[nodiscard]] auto default_format() -> setting<QString>;

[[nodiscard]] auto default_format_def() -> const QString&;

}  // namespace saves

/**
 * @brief Initializes the settings to their default values, if they are absent.
 *
 * @since 0.1.0
 */
void validate();

}  // namespace tactile::prefs

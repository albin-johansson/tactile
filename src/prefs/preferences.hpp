#pragma once

#include <QByteArray>
#include <QPalette>

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

}  // namespace graphics

/**
 * @brief Initializes the settings to their default values, if they are absent.
 *
 * @since 0.1.0
 */
void validate();

}  // namespace tactile::prefs

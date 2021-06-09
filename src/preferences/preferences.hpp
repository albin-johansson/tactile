#pragma once

#include <QByteArray>   // QByteArray
#include <QMap>         // QMap
#include <QPalette>     // QPalette
#include <QString>      // QString
#include <QStringList>  // QStringList
#include <QVariant>     // QVariant

#include "setting.hpp"

/**
 * \namespace tactile::prefs
 *
 * \brief Contains entities related to persistent settings (a.k.a. preferences).
 *
 * \since 0.1.0
 */
namespace tactile::prefs {

/**
 * \namespace tactile::prefs::window
 *
 * \brief Contains window-related settings.
 *
 * \since 0.1.0
 */
namespace window {

/**
 * \brief Returns a setting that represents the last layout geometry of the
 * main window.
 *
 * \return a setting for the last layout geometry of the main window.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto last_layout_geometry() -> setting<QByteArray>;

/**
 * \brief Returns a setting that represents the last layout state of the
 * main window.
 *
 * \return a setting for the last layout state of the main window.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto last_layout_state() -> setting<QByteArray>;

}  // namespace window

/**
 * \namespace tactile::prefs::gfx
 *
 * \brief Contains graphics-related settings.
 *
 * \since 0.1.0
 */
namespace gfx {

// A map with user-defined names and an associated QPalette
using theme_map = QMap<QString, QVariant>;

inline constexpr QColor accent_color_def{84, 153, 199};  // #5499C7
inline constexpr bool render_grid_def = false;
inline constexpr bool tool_widget_visible_def = false;
inline constexpr bool layer_widget_visible_def = true;
inline constexpr bool tileset_widget_visible_def = true;
inline constexpr bool properties_widget_visible_def = true;
inline constexpr bool use_opengl_def = true;

/**
 * \brief Returns a setting that indicates whether or not a grid should be
 * rendered on maps.
 *
 * \return a boolean setting for rendering grids on maps.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto render_grid() -> setting<bool>;

/**
 * \brief Returns a setting that represents the theme of the application.
 *
 * \return a setting that represents the theme of the application.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto theme() -> setting<QPalette>;

[[nodiscard]] auto user_themes() -> setting<theme_map>;

[[nodiscard]] auto user_themes_def() -> theme_map;

/**
 * \brief Returns a setting that represents the name of the application theme.
 *
 * \return a setting that represents the name of the application theme.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto theme_name() -> setting<QString>;

[[nodiscard]] auto accent_color() -> setting<QColor>;

[[nodiscard]] auto tool_widget_visible() -> setting<bool>;

[[nodiscard]] auto layer_widget_visible() -> setting<bool>;

[[nodiscard]] auto tileset_widget_visible() -> setting<bool>;

[[nodiscard]] auto properties_widget_visible() -> setting<bool>;

[[nodiscard]] auto use_opengl() -> setting<bool>;

void reset_tool_widget_visible();

void reset_layer_widget_visible();

void reset_tileset_widget_visible();

void reset_properties_widget_visible();

}  // namespace gfx

namespace saves {

inline constexpr bool embed_tilesets_def = false;
inline constexpr bool generate_defaults_def = false;
inline constexpr bool readable_output_def = true;
inline constexpr int tile_width_def = 32;
inline constexpr int tile_height_def = 32;

[[nodiscard]] auto embed_tilesets() -> setting<bool>;

[[nodiscard]] auto generate_defaults() -> setting<bool>;

[[nodiscard]] auto readable_output() -> setting<bool>;

[[nodiscard]] auto tile_width() -> setting<int>;

[[nodiscard]] auto tile_height() -> setting<int>;

[[nodiscard]] auto default_format() -> setting<QString>;

[[nodiscard]] auto default_format_def() -> const QString&;

}  // namespace saves

/**
 * \brief Initializes the settings to their default values, if they are absent.
 *
 * \since 0.1.0
 */
void validate();

}  // namespace tactile::prefs

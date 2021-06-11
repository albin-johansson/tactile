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

// A map with user-defined names and an associated QPalette
using theme_map = QMap<QString, QVariant>;

inline constexpr QColor accent_color_def{84, 153, 199};  // #5499C7
inline constexpr bool render_grid_def = false;
inline constexpr bool tool_widget_visible_def = false;
inline constexpr bool layer_widget_visible_def = true;
inline constexpr bool tileset_widget_visible_def = true;
inline constexpr bool properties_widget_visible_def = true;
inline constexpr bool use_opengl_def = true;

inline constexpr bool embed_tilesets_def = false;
inline constexpr bool generate_defaults_def = false;
inline constexpr bool readable_output_def = true;
inline constexpr int tile_width_def = 32;
inline constexpr int tile_height_def = 32;

/**
 * \brief Initializes the settings to their default values, if they are absent.
 *
 * \since 0.1.0
 */
void ValidatePreferences();

void ResetToolWidgetVisibility();
void ResetLayerWidgetVisibility();
void ResetTilesetWidgetVisibility();
void ResetPropertiesWidgetVisibility();

/**
 * \brief Returns a setting that represents the last layout geometry of the
 * main window.
 *
 * \return a setting for the last layout geometry of the main window.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto LastLayoutGeometry() -> setting<QByteArray>;

/**
 * \brief Returns a setting that represents the last layout state of the
 * main window.
 *
 * \return a setting for the last layout state of the main window.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto LastLayoutState() -> setting<QByteArray>;

/**
 * \brief Returns a setting that indicates whether or not a grid should be
 * rendered on maps.
 *
 * \return a boolean setting for rendering grids on maps.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto RenderGrid() -> setting<bool>;

/**
 * \brief Returns a setting that represents the Theme of the application.
 *
 * \return a setting that represents the Theme of the application.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto Theme() -> setting<QPalette>;

[[nodiscard]] auto UserThemes() -> setting<theme_map>;

[[nodiscard]] auto UserThemesDefault() -> theme_map;

/**
 * \brief Returns a setting that represents the name of the application Theme.
 *
 * \return a setting that represents the name of the application Theme.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto ThemeName() -> setting<QString>;

[[nodiscard]] auto AccentColor() -> setting<QColor>;

[[nodiscard]] auto ToolWidgetVisibility() -> setting<bool>;

[[nodiscard]] auto LayerWidgetVisibility() -> setting<bool>;

[[nodiscard]] auto TilesetWidgetVisibility() -> setting<bool>;

[[nodiscard]] auto PropertiesWidgetVisibility() -> setting<bool>;

[[nodiscard]] auto UseOpenGl() -> setting<bool>;

[[nodiscard]] auto EmbedTilesets() -> setting<bool>;

[[nodiscard]] auto GenerateDefaults() -> setting<bool>;

[[nodiscard]] auto UseReadableOutput() -> setting<bool>;

[[nodiscard]] auto TileWidth() -> setting<int>;

[[nodiscard]] auto TileHeight() -> setting<int>;

[[nodiscard]] auto DefaultFormat() -> setting<QString>;

[[nodiscard]] auto DefaultFormatDefault() -> const QString&;

}  // namespace tactile::prefs

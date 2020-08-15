#pragma once

#include <QString>
#include <QStringView>

/**
 * @namespace tactile::cfg
 *
 * @brief Provides functions for obtaining keys for various
 * settings/preferences.
 *
 * @since 0.1.0
 */
namespace tactile::cfg {

namespace graphics {

/**
 * @brief Returns the key associated with whether or not a grid is rendered
 * over tilemaps.
 *
 * @return the render grid setting identifier.
 *
 * @since 0.1.0
 */
[[nodiscard]] inline auto grid() -> const QString&
{
  static const QString str{"graphics/grid"};
  return str;
}

}  // namespace graphics

namespace window {

/**
 * @brief Returns the key associated with the last window layout geometry.
 *
 * @return the window layout geometry identifier.
 *
 * @since 0.1.0
 */
[[nodiscard]] inline auto last_layout_geometry() -> const QString&
{
  static const QString str{"window/lastLayoutGeometry"};
  return str;
}

/**
 * @brief Returns the key associated with the last window layout state.
 *
 * @return the window layout state identifier.
 *
 * @since 0.1.0
 */
[[nodiscard]] inline auto last_layout_state() -> const QString&
{
  static const QString str{"window/lastLayoutState"};
  return str;
}

}  // namespace window

}  // namespace tactile::cfg

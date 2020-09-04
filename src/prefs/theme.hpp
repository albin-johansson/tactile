#pragma once

#include <qpalette.h>
#include <qstring.h>
#include <qstringview.h>

#include <optional>

#include "types.hpp"

/**
 * @namespace tactile::theme
 *
 * @brief Provides functions for handling window themes.
 *
 * @since 0.1.0
 */
namespace tactile::theme {

/**
 * @brief Resets the currently active theme to the default theme.
 *
 * @since 0.1.0
 */
void reset();

/**
 * @brief Sets the currently active theme.
 *
 * @details This function has no effect if no palette is associated with the
 * specified theme.
 *
 * @todo Make this function attempt to parse an unknown theme name.
 *
 * @param name the name of the desired theme.
 *
 * @since 0.1.0
 */
void set_theme(const QString& name);

/**
 * @brief Returns the palette associated with the specified name.
 *
 * @param name the name of the theme to obtain the palette of.
 *
 * @return the palette of the specified theme; `std::nullopt` if the theme
 * wasn't recognized.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto from_name(QStringView name) -> std::optional<QPalette>;

/**
 * @brief Returns the palette of the dark theme.
 *
 * @return the palette of the dark theme.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto get_dark() -> const QPalette&;

/**
 * @brief Returns the palette of the light theme.
 *
 * @return the palette of the light theme.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto get_light() -> const QPalette&;

/**
 * @brief Returns the palette of the Atom One Dark theme.
 *
 * @return the palette of the Atom One Dark theme.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto get_atom_one_dark() -> const QPalette&;

/**
 * @brief Returns the palette of the default theme.
 *
 * @return the palette of the default theme.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto get_default() -> const QPalette&;

/**
 * @brief Returns the name of the default theme.
 *
 * @return the name of the default theme.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto get_default_name() -> QStringView;

}  // namespace tactile::theme

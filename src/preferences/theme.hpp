#pragma once

#include <QPalette>     // QPalette
#include <QString>      // QString
#include <QStringView>  // QStringView
#include <vector>       // vector

#include "czstring.hpp"
#include "maybe.hpp"
#include "vector_map.hpp"

/**
 * \namespace tactile::theme
 *
 * \brief Provides functions for handling window themes.
 *
 * \since 0.1.0
 */
namespace tactile {

void validate_themes();

/**
 * \brief Resets the currently active theme to the default theme.
 *
 * \since 0.1.0
 */
void reset_theme();

auto register_theme(const QString& name, const QPalette& palette) -> bool;

/**
 * \brief Sets the currently active theme.
 *
 * \details This function has no effect if no palette is associated with the
 * specified theme.
 *
 * \todo Make this function attempt to parse an unknown theme name.
 *
 * \param name the name of the desired theme.
 *
 * \return `true` if the theme was successfully set; `false` otherwise.
 *
 * \since 0.1.0
 */
auto set_theme(const QString& name) -> bool;

void update_theme(const QString& name,
                  QPalette::ColorRole role,
                  const QColor& color,
                  QPalette::ColorGroup group = QPalette::All);

void remove_theme(const QString& name);

/**
 * \brief Returns the palette associated with the specified name.
 *
 * \param name the name of the theme to obtain the palette of.
 *
 * \return the palette of the specified theme; `std::nullopt` if the theme
 * wasn't recognized.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto get_theme(const QString& name) -> maybe<QPalette>;

/**
 * \brief Indicates whether or not the supplied name is one of the
 * pre-defined themes.
 *
 * \param name the name of the theme that will be checked.
 *
 * \return `true` if the supplied name is associated with a standard theme;
 * `false` otherwise.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto is_standard_theme(QStringView name) -> bool;

[[nodiscard]] auto get_standard_themes() -> vector_map<QString, QPalette>;

[[nodiscard]] auto get_standard_theme_names() -> std::vector<QString>;

[[nodiscard]] auto get_user_theme_names() -> std::vector<QString>;

/**
 * \brief Returns the palette of the default theme.
 *
 * \return the palette of the default theme.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto get_default_theme() -> const QPalette&;

[[nodiscard]] auto get_default_theme_name() -> QStringView;

}  // namespace tactile

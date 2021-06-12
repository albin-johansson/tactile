#pragma once

#include <QPalette>     // QPalette
#include <QString>      // QString
#include <QStringView>  // QStringView
#include <vector>       // vector

#include "czstring.hpp"
#include "maybe.hpp"
#include "vector_map.hpp"

namespace tactile {

void ValidateThemes();

/**
 * \brief Resets the currently active theme to the default theme.
 *
 * \since 0.1.0
 */
void ResetTheme();

auto RegisterTheme(const QString& name, const QPalette& palette) -> bool;

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
auto SetTheme(const QString& name) -> bool;

void UpdateTheme(const QString& name,
                 QPalette::ColorRole role,
                 const QColor& color,
                 QPalette::ColorGroup group = QPalette::All);

void RemoveTheme(const QString& name);

/**
 * \brief Returns the palette associated with the specified name.
 *
 * \param name the name of the theme to obtain the palette of.
 *
 * \return the palette of the specified theme; `nothing` if the theme
 * wasn't recognized.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto GetTheme(const QString& name) -> Maybe<QPalette>;

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
[[nodiscard]] auto IsStandardTheme(QStringView name) -> bool;

[[nodiscard]] auto GetStandardThemes() -> vector_map<QString, QPalette>;

[[nodiscard]] auto GetStandardThemeNames() -> std::vector<QString>;

[[nodiscard]] auto GetUserThemeNames() -> std::vector<QString>;

/**
 * \brief Returns the palette of the default theme.
 *
 * \return the palette of the default theme.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto GetDefaultTheme() -> const QPalette&;

[[nodiscard]] auto GetDefaultThemeName() -> QStringView;

}  // namespace tactile

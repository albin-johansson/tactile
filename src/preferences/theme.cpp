#include "theme.hpp"

#include <QApplication>
#include <array>   // array
#include <map>     // map
#include <ranges>  // any_of, find

#include "parse_palette.hpp"
#include "preferences.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile {
namespace {

inline constexpr QStringView light{u"Light"};
inline constexpr QStringView dark{u"Dark"};
inline constexpr QStringView atomOneDark{u"Atom One Dark"};

inline constexpr auto defaultTheme = dark;

inline constexpr std::array themes{
    std::make_pair(light, QStringView{u":theme/light"}),
    std::make_pair(dark, QStringView{u":theme/dark"}),
    std::make_pair(atomOneDark, QStringView{u":theme/atomOneDark"})};

inline std::map<QStringView, QPalette> palettes;

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
[[nodiscard]] auto is_standard_theme(const QStringView name) -> bool
{
  return std::ranges::any_of(themes, [name](const auto& pair) {
    return pair.first == name;
  });
}

void set_theme(const QString& name, const QPalette& palette)
{
  prefs::gfx::theme().set(palette);
  prefs::gfx::theme_name().set(name);
  QApplication::setPalette(palette);
}

}  // namespace

void validate_themes()
{
  prefs::gfx::theme_name().set_if_missing(defaultTheme.toString());

  for (const auto& [name, path] : themes) {
    const auto palette = parse_palette(path.toString());
    Q_ASSERT(palette);
    palettes.try_emplace(name, palette.value());
  }
}

void reset_theme()
{
  set_theme(defaultTheme.toString());
}

auto register_theme(const QString& name, const QPalette& palette) -> bool
{
  // TODO avoid name collision with standard themes

  auto userThemes = prefs::gfx::user_themes().value();
  if (!userThemes.contains(name)) {
    userThemes.insert(name, palette);
    prefs::gfx::user_themes().set(userThemes);
    return true;
  } else {
    return false;
  }
}

auto set_theme(const QString& name) -> bool
{
  if (const auto palette = get_theme(name)) {
    set_theme(name, *palette);
    return true;
  } else {
    return false;
  }
}

auto get_theme(const QString& name) -> maybe<QPalette>
{
  if (is_standard_theme(name)) {
    return palettes.at(name);

  } else if (const auto userThemes = prefs::gfx::user_themes()) {
    if (const auto it = userThemes->find(name); it != userThemes->end()) {
      Q_ASSERT(it->canConvert<QPalette>());
      return it->value<QPalette>();
    }
  }

  return std::nullopt;
}

auto get_standard_themes() -> vector_map<QString, QPalette>
{
  vector_map<QString, QPalette> map;
  map.reserve(themes.size());

  for (const auto& [name, palette] : palettes) {
    map.emplace(name.toString(), palette);
  }

  return map;
}

auto get_all_theme_names() -> std::vector<QString>
{
  std::vector<QString> names;

  const auto userThemes = prefs::gfx::user_themes();
  names.reserve(userThemes ? palettes.size() + userThemes->size()
                           : palettes.size());

  for (const auto& [name, palette] : palettes) {
    names.emplace_back(name.toString());
  }

  if (userThemes) {
    for (const auto& name : userThemes.value().keys()) {
      names.emplace_back(name);
    }
  }

  return names;
}

auto get_default_theme() -> const QPalette&
{
  return palettes.at(defaultTheme);
}

auto get_default_theme_name() -> QStringView
{
  return defaultTheme;
}

}  // namespace tactile

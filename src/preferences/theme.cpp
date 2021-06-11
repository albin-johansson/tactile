#include "theme.hpp"

#include <QApplication>  // QApplication
#include <array>         // array
#include <map>           // map
#include <ranges>        // any_of, find
#include <utility>       // make_pair

#include "parse_palette.hpp"
#include "preferences.hpp"

namespace tactile {
namespace {

inline constexpr QStringView light{u"Light"};
inline constexpr QStringView dark{u"Dark"};
inline constexpr QStringView atom{u"Atom"};

inline constexpr auto defaultTheme = dark;

inline constexpr std::array themes{
    std::make_pair(light, QStringView{u":theme/light"}),
    std::make_pair(dark, QStringView{u":theme/dark"}),
    std::make_pair(atom, QStringView{u":theme/atom"})};

inline std::map<QStringView, QPalette> palettes;

void set_current_theme(const QString& name, const QPalette& palette)
{
  prefs::gfx::theme() = palette;
  prefs::gfx::theme_name() = name;
  QApplication::setPalette(palette);
}

}  // namespace

void validate_themes()
{
  prefs::gfx::theme_name().set_if_missing(defaultTheme.toString());

  for (const auto& [name, path] : themes)
  {
    const auto palette = ParsePalette(path.toString());
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
  // TODO disallow name collision with standard themes

  auto userThemes = prefs::gfx::user_themes().value();
  if (!userThemes.contains(name))
  {
    userThemes.insert(name, palette);
    prefs::gfx::user_themes() = userThemes;
    return true;
  }
  else
  {
    return false;
  }
}

auto set_theme(const QString& name) -> bool
{
  if (const auto palette = get_theme(name))
  {
    set_current_theme(name, *palette);
    return true;
  }
  else
  {
    return false;
  }
}

void update_theme(const QString& name,
                  const QPalette::ColorRole role,
                  const QColor& color,
                  const QPalette::ColorGroup group)
{
  // TODO don't allow updating a standard theme

  if (auto palette = get_theme(name))
  {
    palette->setColor(group, role, color);

    if (is_standard_theme(name))
    {
      palettes.at(name) = *palette;
    }
    else
    {
      auto userThemes = prefs::gfx::user_themes().value();
      userThemes.insert(name, *palette);
      prefs::gfx::user_themes() = userThemes;
    }
  }
}

void remove_theme(const QString& name)
{
  Q_ASSERT(!is_standard_theme(name));
  if (const auto userThemes = prefs::gfx::user_themes())
  {
    auto map = userThemes.value();
    map.remove(name);
    prefs::gfx::user_themes() = map;
  }
}

auto get_theme(const QString& name) -> maybe<QPalette>
{
  if (is_standard_theme(name))
  {
    return palettes.at(name);
  }
  else if (const auto userThemes = prefs::gfx::user_themes())
  {
    if (const auto it = userThemes->find(name); it != userThemes->end())
    {
      Q_ASSERT(it->canConvert<QPalette>());
      return it->value<QPalette>();
    }
  }

  return std::nullopt;
}

auto is_standard_theme(const QStringView name) -> bool
{
  return std::ranges::any_of(themes, [name](const auto& pair) {
    return pair.first == name;
  });
}

auto get_standard_themes() -> vector_map<QString, QPalette>
{
  vector_map<QString, QPalette> map;
  map.reserve(themes.size());

  for (const auto& [name, palette] : palettes)
  {
    map.emplace(name.toString(), palette);
  }

  return map;
}

auto get_standard_theme_names() -> std::vector<QString>
{
  std::vector<QString> names;
  names.reserve(palettes.size());

  for (const auto& [name, palette] : palettes)
  {
    names.emplace_back(name.toString());
  }

  return names;
}

auto get_user_theme_names() -> std::vector<QString>
{
  std::vector<QString> names;

  if (const auto userThemes = prefs::gfx::user_themes())
  {
    names.reserve(userThemes->size());
    for (const auto& name : userThemes.value().keys())
    {
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

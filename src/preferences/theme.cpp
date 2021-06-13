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
  prefs::Theme() = palette;
  prefs::ThemeName() = name;
  QApplication::setPalette(palette);
}

}  // namespace

void ValidateThemes()
{
  prefs::ThemeName().SetIfMissing(defaultTheme.toString());

  for (const auto& [name, path] : themes)
  {
    const auto palette = ParsePalette(path.toString());
    Q_ASSERT(palette);
    palettes.try_emplace(name, palette.value());
  }
}

void ResetTheme()
{
  SetTheme(defaultTheme.toString());
}

auto RegisterTheme(const QString& name, const QPalette& palette) -> bool
{
  // TODO disallow name collision with standard themes

  auto userThemes = prefs::UserThemes().Value();
  if (!userThemes.contains(name))
  {
    userThemes.insert(name, palette);
    prefs::UserThemes() = userThemes;
    return true;
  }
  else
  {
    return false;
  }
}

auto SetTheme(const QString& name) -> bool
{
  if (const auto palette = GetTheme(name))
  {
    set_current_theme(name, *palette);
    return true;
  }
  else
  {
    return false;
  }
}

void UpdateTheme(const QString& name,
                 QPalette::ColorRole role,
                 const QColor& color,
                 QPalette::ColorGroup group)
{
  // TODO don't allow updating a standard theme

  if (auto palette = GetTheme(name))
  {
    palette->setColor(group, role, color);

    if (IsStandardTheme(name))
    {
      palettes.at(name) = *palette;
    }
    else
    {
      auto userThemes = prefs::UserThemes().Value();
      userThemes.insert(name, *palette);
      prefs::UserThemes() = userThemes;
    }
  }
}

void RemoveTheme(const QString& name)
{
  Q_ASSERT(!IsStandardTheme(name));
  if (const auto userThemes = prefs::UserThemes())
  {
    auto map = userThemes.Value();
    map.remove(name);
    prefs::UserThemes() = map;
  }
}

auto GetTheme(const QString& name) -> Maybe<QPalette>
{
  if (IsStandardTheme(name))
  {
    return palettes.at(name);
  }
  else if (const auto userThemes = prefs::UserThemes())
  {
    if (const auto it = userThemes->find(name); it != userThemes->end())
    {
      Q_ASSERT(it->canConvert<QPalette>());
      return it->value<QPalette>();
    }
  }

  return nothing;
}

auto IsStandardTheme(QStringView name) -> bool
{
  return std::ranges::any_of(themes, [name](const auto& pair) {
    return pair.first == name;
  });
}

auto GetStandardThemes() -> vector_map<QString, QPalette>
{
  vector_map<QString, QPalette> map;
  map.reserve(themes.size());

  for (const auto& [name, palette] : palettes)
  {
    map.emplace(name.toString(), palette);
  }

  return map;
}

auto GetStandardThemeNames() -> std::vector<QString>
{
  std::vector<QString> names;
  names.reserve(palettes.size());

  for (const auto& [name, palette] : palettes)
  {
    names.emplace_back(name.toString());
  }

  return names;
}

auto GetUserThemeNames() -> std::vector<QString>
{
  std::vector<QString> names;

  if (const auto userThemes = prefs::UserThemes())
  {
    names.reserve(userThemes->size());
    for (const auto& name : userThemes.Value().keys())
    {
      names.emplace_back(name);
    }
  }

  return names;
}

auto GetDefaultTheme() -> const QPalette&
{
  return palettes.at(defaultTheme);
}

auto GetDefaultThemeName() -> QStringView
{
  return defaultTheme;
}

}  // namespace tactile

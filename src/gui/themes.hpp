#pragma once

#include <string_view>  // string_view

struct ImGuiStyle;

namespace Tactile {

inline constexpr auto theme_options = "Dear Dark\0Dear Light\0Ash\0\0";

enum class Theme
{
  DearDark = 0,
  DearLight = 1,
  Ash = 2
};

void ApplyTheme(ImGuiStyle& style, Theme theme);

[[nodiscard]] auto GetThemeFromIndex(int index) -> std::string;

[[nodiscard]] auto GetThemeFromName(std::string_view name) -> Theme;

[[nodiscard]] auto GetThemeIndex(Theme theme) -> int;

[[nodiscard]] auto GetThemeIndex(std::string_view name) -> int;

}  // namespace Tactile

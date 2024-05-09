// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/model/settings.hpp"

namespace tactile {
inline namespace settings {

inline constexpr float kDefaultFontSize = 13.0f;

}  // namespace settings

auto get_default_settings() -> Settings
{
  return Settings {
    .font_size = kDefaultFontSize,
  };
}

}  // namespace tactile

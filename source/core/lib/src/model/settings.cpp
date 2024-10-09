// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/model/settings.hpp"

namespace tactile::core {
inline namespace settings {

inline constexpr auto kLanguageDefault = LanguageID::kAmericanEnglish;
inline constexpr auto kCommandCapacityDefault = std::size_t {100};
inline constexpr auto kFontDefault = ui::FontID::kDefault;
inline constexpr auto kFontSizeDefault = 13.0f;
inline constexpr auto kLogVerboseEventsDefault = false;

}  // namespace settings

auto get_default_settings() -> Settings
{
  return Settings {
    .language = kLanguageDefault,
    .command_capacity = kCommandCapacityDefault,
    .font = kFontDefault,
    .font_size = kFontSizeDefault,
    .log_verbose_events = kLogVerboseEventsDefault,
  };
}

}  // namespace tactile::core

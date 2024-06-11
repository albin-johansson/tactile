// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/model/settings.hpp"

namespace tactile {
inline namespace settings {

inline constexpr LanguageID kLanguageDefault = LanguageID::kAmericanEnglish;
inline constexpr usize kCommandCapacityDefault = 100;
inline constexpr float kFontSizeDefault = 13.0f;
inline constexpr bool kLogVerboseEventsDefault = false;

}  // namespace settings

auto get_default_settings() -> Settings
{
  return Settings {
    .language = kLanguageDefault,
    .command_capacity = kCommandCapacityDefault,
    .font_size = kFontSizeDefault,
    .log_verbose_events = kLogVerboseEventsDefault,
  };
}

}  // namespace tactile

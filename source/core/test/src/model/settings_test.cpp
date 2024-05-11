// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/model/settings.hpp"

#include <gtest/gtest.h>

namespace tactile {

/// \trace tactile::get_default_settings
TEST(Settings, GetDefaultSettings)
{
  const auto settings = get_default_settings();
  EXPECT_EQ(settings.language, LanguageID::kAmericanEnglish);
  EXPECT_EQ(settings.font_size, 13.0f);
  EXPECT_EQ(settings.log_verbose_events, false);
}

}  // namespace tactile

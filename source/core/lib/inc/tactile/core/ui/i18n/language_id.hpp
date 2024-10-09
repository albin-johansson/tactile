// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile::core {

/**
 * Provides identifiers for the supported languages.
 */
enum class LanguageID : std::uint8_t
{
  kAmericanEnglish,
  kBritishEnglish,
  kSwedish,
};

}  // namespace tactile::core

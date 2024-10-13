// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>  // size_t
#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/fonts.hpp"

namespace tactile::core {

namespace ui {
enum class LanguageID : std::uint8_t;
}  // namespace ui

/**
 * Represents editor settings that the user can control.
 */
struct Settings final
{
  /** The language that will be loaded at startup. */
  ui::LanguageID language;

  /** The maximum number of changes to track in a document. */
  std::size_t command_capacity;

  /** The font used in the UI. */
  ui::FontID font;

  /** The UI font size. */
  float font_size;

  /** Whether verbose events (e.g., some mouse events) should be logged. */
  bool log_verbose_events : 1;
};

/**
 * Returns the settings used by default.
 *
 * \return
 * The default settings.
 */
[[nodiscard]]
auto get_default_settings() -> Settings;

}  // namespace tactile::core

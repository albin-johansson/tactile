// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * RAII utility for modifying a style variable for the duration of a scope.
 */
class TACTILE_CORE_API ScopedStyle final {
 public:
  TACTILE_DELETE_COPY(ScopedStyle);
  TACTILE_DELETE_MOVE(ScopedStyle);

  /**
   * Updates a float style variable.
   *
   * \param style_var The style variable index.
   * \param value     The new style value.
   */
  ScopedStyle(ImGuiStyleVar style_var, float value);

  /**
   * Updates a vector style variable.
   *
   * \param style_var The style variable index.
   * \param value     The new style value.
   */
  ScopedStyle(ImGuiStyleVar style_var, const ImVec2& value);

  /**
   * Resets the style variable.
   */
  ~ScopedStyle() noexcept;
};

/**
 * Applies our custom style options to a style object.
 *
 * \param style The style object to update.
 */
void apply_custom_style(ImGuiStyle& style);

}  // namespace tactile::core

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>    // max
#include <type_traits>  // is_same_v

#include <imgui.h>

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/**
 * RAII helper for the PushStyleVar/PopStyleVar ImGui functions.
 */
class StyleVarScope final
{
 public:
  TACTILE_DELETE_COPY(StyleVarScope);
  TACTILE_DELETE_MOVE(StyleVarScope);

  [[nodiscard]]
  StyleVarScope(ImGuiStyleVar id, ImVec2 value);

  [[nodiscard]]
  StyleVarScope(ImGuiStyleVar id, float value);

  ~StyleVarScope() noexcept;

  void pop() noexcept;

 private:
  bool mPopped {false};
};

/**
 * RAII helper for the PushStyleColor/PopStyleColor ImGui functions.
 */
class StyleColorScope final
{
 public:
  TACTILE_DELETE_COPY(StyleColorScope);
  TACTILE_DELETE_MOVE(StyleColorScope);

  [[nodiscard]]
  StyleColorScope(ImGuiCol id, const ImVec4& color);

  [[nodiscard]]
  StyleColorScope(ImGuiCol id, uint32 color);

  ~StyleColorScope() noexcept;

  void pop() noexcept;

 private:
  bool mPopped {false};
};

/**
 * Applies the custom editor style options to a style descriptor.
 *
 * \param style The target style descriptor.
 */
void apply_custom_style(ImGuiStyle& style);

/**
 * Returns the minimum offset required to align labeled widgets.
 *
 * \tparam Args The string pack types.
 *
 * \param strings The strings that will be measured.
 *
 * \return
 * The offset required to align widgets placed after the provided strings.
 */
template <typename... Args>
  requires((std::is_same_v<Args, const char*> && ... && true))
[[nodiscard]] auto get_alignment_offset(Args... strings) -> float
{
  const auto& style = ImGui::GetStyle();
  const auto spacing = style.ItemSpacing.x * 2.0f;
  return spacing + std::max({ImGui::CalcTextSize(strings).x...});
}

}  // namespace tactile::ui

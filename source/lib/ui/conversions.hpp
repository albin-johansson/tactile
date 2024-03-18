// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <bit>      // bit_cast
#include <cstdint>  // uintptr_t

#include <imgui.h>

#include "tactile/base/int.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/meta/color.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

[[nodiscard]] constexpr auto to_u32(const Color& color) noexcept -> uint32
{
  return IM_COL32(color.red, color.green, color.blue, color.alpha);
}

[[nodiscard]] inline auto to_color(const ImVec4& vec) -> Color
{
  return Color::from(vec.x, vec.y, vec.z, vec.w);
}

[[nodiscard]] constexpr auto as_imvec2(const Float2 vec) noexcept -> ImVec2
{
  return {vec.x(), vec.y()};
}

[[nodiscard]] constexpr auto as_imvec2(const Int2 vec) noexcept -> ImVec2
{
  return {static_cast<float>(vec.x()), static_cast<float>(vec.y())};
}

[[nodiscard]] constexpr auto as_float2(const ImVec2 vec) noexcept -> Float2
{
  return {vec.x, vec.y};
}

/// Converts a normalized opacity value to a value in the interval [0, 255].
[[nodiscard]] constexpr auto opacity_cast(const float opacity) noexcept -> uint8
{
  TACTILE_ASSERT(opacity >= 0.0f);
  TACTILE_ASSERT(opacity <= 1.0f);
  return static_cast<uint8>(255.0f * opacity);
}

/// Converts an OpenGL texture identifier to a Dear ImGui texture identifier.
[[nodiscard]] inline auto to_imgui_texture_id(const uint texture_id) noexcept
    -> ImTextureID
{
  return std::bit_cast<ImTextureID>(static_cast<std::uintptr_t>(texture_id));
}

}  // namespace tactile

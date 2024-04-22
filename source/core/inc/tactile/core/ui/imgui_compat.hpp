// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/**
 * Creates a Tactile vector from an ImGui vector.
 *
 * \param vec The vector to convert.
 *
 * \return
 * A Tactile vector.
 */
[[nodiscard]]
constexpr auto to_float2(const ImVec2& vec) noexcept -> Float2
{
  return Float2 {vec.x, vec.y};
}

/**
 * \copydoc to_float2
 */
[[nodiscard]]
constexpr auto to_float4(const ImVec4& vec) noexcept -> Float4
{
  return Float4 {vec.x, vec.y, vec.z, vec.w};
}

/**
 * \copydoc to_float2
 */
[[nodiscard]]
constexpr auto to_int2(const ImVec2& vec) noexcept -> Int2
{
  return Int2 {static_cast<Int2::value_type>(vec.x),
               static_cast<Int2::value_type>(vec.y)};
}

/**
 * \copydoc to_float2
 */
[[nodiscard]]
constexpr auto to_int4(const ImVec4& vec) noexcept -> Int4
{
  return Int4 {static_cast<Int2::value_type>(vec.x),
               static_cast<Int2::value_type>(vec.y),
               static_cast<Int2::value_type>(vec.z),
               static_cast<Int2::value_type>(vec.w)};
}

/**
 * Creates an ImGui vector from a Tactile vector.
 *
 * \param vec The vector to convert.
 *
 * \return
 * An ImGui vector.
 */
[[nodiscard]]
constexpr auto to_imvec2(const Float2& vec) noexcept -> ImVec2
{
  return ImVec2 {vec.x(), vec.y()};
}

/**
 * \copydoc to_imvec2
 */
[[nodiscard]]
constexpr auto to_imvec2(const Int2& vec) noexcept -> ImVec2
{
  return ImVec2 {static_cast<float>(vec.x()), static_cast<float>(vec.y())};
}

/**
 * \copydoc to_imvec2
 */
[[nodiscard]]
constexpr auto to_imvec4(const Float4& vec) noexcept -> ImVec4
{
  return ImVec4 {vec.x(), vec.y(), vec.z(), vec.w()};
}

/**
 * \copydoc to_imvec2
 */
[[nodiscard]]
constexpr auto to_imvec4(const Int4& vec) noexcept -> ImVec4
{
  return ImVec4 {static_cast<float>(vec.x()),
                 static_cast<float>(vec.y()),
                 static_cast<float>(vec.z()),
                 static_cast<float>(vec.w())};
}

}  // namespace tactile

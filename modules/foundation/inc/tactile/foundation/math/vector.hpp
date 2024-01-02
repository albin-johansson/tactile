// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <glm/gtx/io.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "tactile/foundation/prelude.hpp"

namespace tactile {

using Int2 = glm::vec<2, int32>;
using Int3 = glm::vec<3, int32>;
using Int4 = glm::vec<4, int32>;

using Float2 = glm::vec2;
using Float3 = glm::vec3;
using Float4 = glm::vec4;

struct VectorComparisonResult final {
  bool same_x : 1 {};
  bool same_y : 1 {};
  bool same_z : 1 {};
  bool same_w : 1 {};
};

/** Performs a component-wise comparison of two vectors. */
template <typename VectorType>
[[nodiscard]] constexpr auto compare_vectors(const VectorType& a,
                                             const VectorType& b) noexcept
    -> VectorComparisonResult
{
  VectorComparisonResult result;
  result.same_x = a.x == b.x;
  result.same_y = a.y == b.y;

  if constexpr (VectorType::length() >= 3) {
    result.same_z = a[2] == b[2];
  }

  if constexpr (VectorType::length() >= 4) {
    result.same_w = a[3] == b[3];
  }

  return result;
}

}  // namespace tactile

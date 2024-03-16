// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

[[nodiscard]]
auto serialize(const Int2& vec) -> String;

[[nodiscard]]
auto serialize(const Float2& vec) -> String;

[[nodiscard]]
auto serialize(const Int3& vec) -> String;

[[nodiscard]]
auto serialize(const Float3& vec) -> String;

[[nodiscard]]
auto serialize(const Int4& vec) -> String;

[[nodiscard]]
auto serialize(const Float4& vec) -> String;

[[nodiscard]]
auto deserialize_int2(StringView str) -> Result<Int2>;

[[nodiscard]]
auto deserialize_int3(StringView str) -> Result<Int3>;

[[nodiscard]]
auto deserialize_int4(StringView str) -> Result<Int4>;

[[nodiscard]]
auto deserialize_float2(StringView str) -> Result<Float2>;

[[nodiscard]]
auto deserialize_float3(StringView str) -> Result<Float3>;

[[nodiscard]]
auto deserialize_float4(StringView str) -> Result<Float4>;

}  // namespace tactile

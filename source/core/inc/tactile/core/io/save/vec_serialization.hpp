// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code

#include "tactile/base/container/string.hpp"
#include "tactile/base/numeric/vec.hpp"

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
auto deserialize_int2(StringView str) -> std::expected<Int2, std::error_code>;

[[nodiscard]]
auto deserialize_int3(StringView str) -> std::expected<Int3, std::error_code>;

[[nodiscard]]
auto deserialize_int4(StringView str) -> std::expected<Int4, std::error_code>;

[[nodiscard]]
auto deserialize_float2(StringView str) -> std::expected<Float2, std::error_code>;

[[nodiscard]]
auto deserialize_float3(StringView str) -> std::expected<Float3, std::error_code>;

[[nodiscard]]
auto deserialize_float4(StringView str) -> std::expected<Float4, std::error_code>;

}  // namespace tactile

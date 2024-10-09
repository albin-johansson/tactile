// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>     // expected
#include <string>       // string
#include <string_view>  // string_view

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/numeric/vec.hpp"

namespace tactile::core {

[[nodiscard]]
auto serialize(const Int2& vec) -> std::string;

[[nodiscard]]
auto serialize(const Float2& vec) -> std::string;

[[nodiscard]]
auto serialize(const Int3& vec) -> std::string;

[[nodiscard]]
auto serialize(const Float3& vec) -> std::string;

[[nodiscard]]
auto serialize(const Int4& vec) -> std::string;

[[nodiscard]]
auto serialize(const Float4& vec) -> std::string;

[[nodiscard]]
auto deserialize_int2(std::string_view str) -> std::expected<Int2, ErrorCode>;

[[nodiscard]]
auto deserialize_int3(std::string_view str) -> std::expected<Int3, ErrorCode>;

[[nodiscard]]
auto deserialize_int4(std::string_view str) -> std::expected<Int4, ErrorCode>;

[[nodiscard]]
auto deserialize_float2(std::string_view str) -> std::expected<Float2, ErrorCode>;

[[nodiscard]]
auto deserialize_float3(std::string_view str) -> std::expected<Float3, ErrorCode>;

[[nodiscard]]
auto deserialize_float4(std::string_view str) -> std::expected<Float4, ErrorCode>;

}  // namespace tactile::core

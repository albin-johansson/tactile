// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/save/vec_serialization.hpp"

#include <format>  // format

#include "tactile/base/io/int_parser.hpp"
#include "tactile/core/util/string_conv.hpp"
#include "tactile/core/util/string_ops.hpp"

namespace tactile::core {
namespace {

template <typename VecType>
[[nodiscard]] auto _deserialize_vec(const std::string_view str)
    -> std::expected<VecType, ErrorCode>
{
  VecType vec {};
  std::size_t index = 0;

  const auto ok = split_string(str, ';', [&](const std::string_view token) {
    if (index >= vec.size()) {
      return false;
    }

    using tactile::parse;
    using tactile::core::parse;

    const auto parsed_value = parse<typename VecType::value_type>(token);
    if (!parsed_value.has_value()) {
      return false;
    }

    vec[index] = *parsed_value;
    ++index;

    return true;
  });

  if (!ok) {
    return std::unexpected {ErrorCode::kBadParam};
  }

  return vec;
}

}  // namespace

auto serialize(const Int2& vec) -> std::string
{
  return std::format("{};{}", vec.x(), vec.y());
}

auto serialize(const Float2& vec) -> std::string
{
  return std::format("{};{}", vec.x(), vec.y());
}

auto serialize(const Int3& vec) -> std::string
{
  return std::format("{};{};{}", vec.x(), vec.y(), vec.z());
}

auto serialize(const Float3& vec) -> std::string
{
  return std::format("{};{};{}", vec.x(), vec.y(), vec.z());
}

auto serialize(const Int4& vec) -> std::string
{
  return std::format("{};{};{};{}", vec.x(), vec.y(), vec.z(), vec.w());
}

auto serialize(const Float4& vec) -> std::string
{
  return std::format("{};{};{};{}", vec.x(), vec.y(), vec.z(), vec.w());
}

auto deserialize_int2(const std::string_view str) -> std::expected<Int2, ErrorCode>
{
  return _deserialize_vec<Int2>(str);
}

auto deserialize_int3(const std::string_view str) -> std::expected<Int3, ErrorCode>
{
  return _deserialize_vec<Int3>(str);
}

auto deserialize_int4(const std::string_view str) -> std::expected<Int4, ErrorCode>
{
  return _deserialize_vec<Int4>(str);
}

auto deserialize_float2(const std::string_view str) -> std::expected<Float2, ErrorCode>
{
  return _deserialize_vec<Float2>(str);
}

auto deserialize_float3(const std::string_view str) -> std::expected<Float3, ErrorCode>
{
  return _deserialize_vec<Float3>(str);
}

auto deserialize_float4(const std::string_view str) -> std::expected<Float4, ErrorCode>
{
  return _deserialize_vec<Float4>(str);
}

}  // namespace tactile::core

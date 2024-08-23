// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/save/vec_serialization.hpp"

#include <format>  // format

#include "tactile/base/io/int_parser.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/util/string_conv.hpp"
#include "tactile/core/util/string_ops.hpp"

namespace tactile {
inline namespace vec_serialization {

template <typename VecType>
[[nodiscard]] auto _deserialize_vec(const StringView str)
    -> std::expected<VecType, std::error_code>
{
  VecType vec {};
  usize index = 0;

  const auto ok = split_string(str, ';', [&](const StringView token) {
    if (index >= vec.size()) {
      return false;
    }

    const auto parsed_value = parse<typename VecType::value_type>(token);
    if (!parsed_value.has_value()) {
      return false;
    }

    vec[index] = *parsed_value;
    ++index;

    return true;
  });

  if (!ok) {
    return std::unexpected {make_error(GenericError::kInvalidParam)};
  }

  return vec;
}

}  // namespace vec_serialization

auto serialize(const Int2& vec) -> String
{
  return std::format("{};{}", vec.x(), vec.y());
}

auto serialize(const Float2& vec) -> String
{
  return std::format("{};{}", vec.x(), vec.y());
}

auto serialize(const Int3& vec) -> String
{
  return std::format("{};{};{}", vec.x(), vec.y(), vec.z());
}

auto serialize(const Float3& vec) -> String
{
  return std::format("{};{};{}", vec.x(), vec.y(), vec.z());
}

auto serialize(const Int4& vec) -> String
{
  return std::format("{};{};{};{}", vec.x(), vec.y(), vec.z(), vec.w());
}

auto serialize(const Float4& vec) -> String
{
  return std::format("{};{};{};{}", vec.x(), vec.y(), vec.z(), vec.w());
}

auto deserialize_int2(const StringView str) -> std::expected<Int2, std::error_code>
{
  return _deserialize_vec<Int2>(str);
}

auto deserialize_int3(const StringView str) -> std::expected<Int3, std::error_code>
{
  return _deserialize_vec<Int3>(str);
}

auto deserialize_int4(const StringView str) -> std::expected<Int4, std::error_code>
{
  return _deserialize_vec<Int4>(str);
}

auto deserialize_float2(const StringView str) -> std::expected<Float2, std::error_code>
{
  return _deserialize_vec<Float2>(str);
}

auto deserialize_float3(const StringView str) -> std::expected<Float3, std::error_code>
{
  return _deserialize_vec<Float3>(str);
}

auto deserialize_float4(const StringView str) -> std::expected<Float4, std::error_code>
{
  return _deserialize_vec<Float4>(str);
}

}  // namespace tactile

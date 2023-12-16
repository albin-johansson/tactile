// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/save/vector_deserialization.hpp"

#include <type_traits>  // is_same_v

namespace tactile {
namespace {

template <typename T>
[[nodiscard]] auto _parse_vector(const StringView str) -> Maybe<T>
{
  using scalar_type = typename T::value_type;

  const auto values = str_split(str, ';');

  T vec {};
  if (values.size() != vec.length()) {
    return kNone;
  }

  usize index = 0;
  for (const auto& value : values) {
    Maybe<scalar_type> parsed_value;

    if constexpr (std::is_same_v<scalar_type, float>) {
      parsed_value = str_to_f32(value);
    }
    else {
      parsed_value = str_to_i32(value);
    }

    if (parsed_value.has_value()) {
      vec[index] = *parsed_value;
    }
    else {
      return kNone;
    }

    ++index;
  }

  return vec;
}

}  // namespace

auto deserialize_int2(const StringView str) -> Maybe<Int2>
{
  return _parse_vector<Int2>(str);
}

auto deserialize_int3(const StringView str) -> Maybe<Int3>
{
  return _parse_vector<Int3>(str);
}

auto deserialize_int4(const StringView str) -> Maybe<Int4>
{
  return _parse_vector<Int4>(str);
}

auto deserialize_float2(const StringView str) -> Maybe<Float2>
{
  return _parse_vector<Float2>(str);
}

auto deserialize_float3(const StringView str) -> Maybe<Float3>
{
  return _parse_vector<Float3>(str);
}

auto deserialize_float4(const StringView str) -> Maybe<Float4>
{
  return _parse_vector<Float4>(str);
}

}  // namespace tactile

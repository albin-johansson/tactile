// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>      // uint8_t
#include <expected>     // expected
#include <ostream>      // ostream
#include <stdexcept>    // invalid_argument
#include <string_view>  // string_view

#include "tactile/base/debug/error_code.hpp"

namespace tactile {

/**
 * The supported attribute value types.
 */
enum class AttributeType : std::uint8_t
{
  kStr,
  kInt,
  kInt2,
  kInt3,
  kInt4,
  kFloat,
  kFloat2,
  kFloat3,
  kFloat4,
  kBool,
  kPath,
  kColor,
  kObject,
};

/**
 * Parses an attribute type from a string.
 *
 * \param str The attribute type name.
 *
 * \return
 * An attribute type if successful; an error code otherwise.
 */
[[nodiscard]]
constexpr auto parse_attribute_type(const std::string_view str)
    -> std::expected<AttributeType, ErrorCode>
{
  if (str == "string") {
    return AttributeType::kStr;
  }

  if (str == "int") {
    return AttributeType::kInt;
  }

  if (str == "int2") {
    return AttributeType::kInt2;
  }

  if (str == "int3") {
    return AttributeType::kInt3;
  }

  if (str == "int4") {
    return AttributeType::kInt4;
  }

  if (str == "float") {
    return AttributeType::kFloat;
  }

  if (str == "float2") {
    return AttributeType::kFloat2;
  }

  if (str == "float3") {
    return AttributeType::kFloat3;
  }

  if (str == "float4") {
    return AttributeType::kFloat4;
  }

  if (str == "bool") {
    return AttributeType::kBool;
  }

  if (str == "path" || str == "file") {
    return AttributeType::kPath;
  }

  if (str == "color") {
    return AttributeType::kColor;
  }

  if (str == "object") {
    return AttributeType::kObject;
  }

  return std::unexpected {ErrorCode::kParseError};
}

/**
 * Converts an attribute type to a string.
 *
 * \details
 * The returned string is guaranteed to be null-terminated.
 *
 * \param type An attribute type.
 *
 * \return
 * A string.
 */
[[nodiscard]]
constexpr auto to_string(const AttributeType type) -> std::string_view
{
  switch (type) {
    case AttributeType::kStr:    return "string";
    case AttributeType::kInt:    return "int";
    case AttributeType::kInt2:   return "int2";
    case AttributeType::kInt3:   return "int3";
    case AttributeType::kInt4:   return "int4";
    case AttributeType::kFloat:  return "float";
    case AttributeType::kFloat2: return "float2";
    case AttributeType::kFloat3: return "float3";
    case AttributeType::kFloat4: return "float4";
    case AttributeType::kBool:   return "bool";
    case AttributeType::kPath:   return "path";
    case AttributeType::kColor:  return "color";
    case AttributeType::kObject: return "object";
  }

  throw std::invalid_argument {"bad attribute type"};
}

/**
 * Outputs an attribute type to a stream for debugging purposes.
 *
 * \param stream The output stream to use.
 * \param type   The attribute type to output.
 *
 * \return
 * The provided stream.
 */
inline auto operator<<(std::ostream& stream, const AttributeType type) -> std::ostream&
{
  return stream << to_string(type);
}

}  // namespace tactile

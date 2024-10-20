#pragma once

#include <algorithm>    // contains
#include <concepts>     // signed_integral, unsigned_integral, floating_point, same_as
#include <expected>     // expected, unexpected
#include <filesystem>   // path
#include <limits>       // numeric_limits
#include <optional>     // optional, nullopt
#include <span>         // span
#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // move

#include <pugixml.hpp>

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/io/compress/compression_format_id.hpp"
#include "tactile/base/layer/layer_type.hpp"
#include "tactile/base/meta/attribute_type.hpp"
#include "tactile/base/numeric/conversion.hpp"
#include "tactile/tiled_tmx/api.hpp"

namespace tactile::tiled_tmx {

[[nodiscard]]
TACTILE_TILED_TMX_API auto read_xml_document(const std::filesystem::path& path)
    -> std::expected<pugi::xml_document, ErrorCode>;

[[nodiscard]]
TACTILE_TILED_TMX_API auto save_xml_document(const pugi::xml_document& document,
                                             const std::filesystem::path& path)
    -> std::expected<void, ErrorCode>;

[[nodiscard]]
TACTILE_TILED_TMX_API auto read_property_type(std::string_view name)
    -> std::expected<AttributeType, ErrorCode>;

[[nodiscard]]
TACTILE_TILED_TMX_API auto get_property_type_name(AttributeType type) -> const char*;

[[nodiscard]]
TACTILE_TILED_TMX_API auto read_layer_type(std::string_view name)
    -> std::expected<LayerType, ErrorCode>;

[[nodiscard]]
TACTILE_TILED_TMX_API auto get_layer_type_name(LayerType type) -> const char*;

[[nodiscard]]
TACTILE_TILED_TMX_API auto read_compression_format(std::string_view name)
    -> std::expected<std::optional<CompressionFormatId>, ErrorCode>;

[[nodiscard]]
TACTILE_TILED_TMX_API auto get_compression_format_name(CompressionFormatId format)  //
    -> const char*;

template <typename T, std::invocable<const pugi::xml_node&> Parser>
[[nodiscard]] auto read_nodes(const pugi::xml_node& parent_node,
                              const std::span<const std::string_view> node_names,
                              const Parser& value_parser)
    -> std::expected<std::vector<T>, ErrorCode>
{
  std::vector<T> values {};

  for (const auto& node : parent_node.children()) {
    if (!std::ranges::contains(node_names, node.name())) {
      continue;
    }

    auto value = value_parser(node);

    if (!value.has_value()) {
      return std::unexpected {value.error()};
    }

    values.push_back(std::move(*value));
  }

  return std::move(values);
}

template <typename T, std::invocable<const pugi::xml_node&> Parser>
[[nodiscard]] auto read_nodes(const pugi::xml_node& parent_node,
                              const std::string_view node_name,
                              const Parser& value_parser)
    -> std::expected<std::vector<T>, ErrorCode>
{
  return read_nodes<T>(parent_node, std::span {&node_name, 1}, value_parser);
}

template <typename T>
[[nodiscard]] auto read_attr(const pugi::xml_node& node, const char* name) -> std::optional<T>;

template <>
[[nodiscard]] inline auto read_attr<std::string>(const pugi::xml_node& node, const char* name)
    -> std::optional<std::string>
{
  if (const auto* str = node.attribute(name).as_string(nullptr)) {
    return std::string {str};
  }

  return std::nullopt;
}

template <>
[[nodiscard]] inline auto read_attr<bool>(const pugi::xml_node& node, const char* name)
    -> std::optional<bool>
{
  if (const char* str = node.attribute(name).as_string(nullptr)) {
    const std::string_view str_view {str};

    if (str_view == "true") {
      return true;
    }

    if (str_view == "false") {
      return false;
    }
  }

  return std::nullopt;
}

template <std::signed_integral T>
  requires(!std::same_as<T, bool>)
[[nodiscard]] auto read_attr(const pugi::xml_node& node, const char* name) -> std::optional<T>
{
  constexpr auto sentinel = std::numeric_limits<long long>::max();

  if (const auto value = node.attribute(name).as_llong(sentinel); value != sentinel) {
    return narrow<T>(value);
  }

  return std::nullopt;
}

template <std::unsigned_integral T>
  requires(!std::same_as<T, bool>)
[[nodiscard]] auto read_attr(const pugi::xml_node& node, const char* name) -> std::optional<T>
{
  constexpr auto sentinel = std::numeric_limits<unsigned long long>::max();

  if (const auto value = node.attribute(name).as_ullong(sentinel); value != sentinel) {
    return narrow<T>(value);
  }

  return std::nullopt;
}

template <std::floating_point T>
[[nodiscard]] auto read_attr(const pugi::xml_node& node, const char* name) -> std::optional<T>
{
  constexpr auto sentinel = std::numeric_limits<double>::max();

  if (const auto value = node.attribute(name).as_double(sentinel); value != sentinel) {
    return static_cast<T>(value);
  }

  return std::nullopt;
}

template <typename T>
[[nodiscard]] auto read_attr_to(const pugi::xml_node& node, const char* name, T& result)
    -> std::expected<void, ErrorCode>
{
  if (auto value = read_attr<T>(node, name)) {
    result = std::move(*value);
    return {};
  }

  return std::unexpected {ErrorCode::kParseError};
}

}  // namespace tactile::tiled_tmx

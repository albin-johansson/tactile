// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // signed_integral, unsigned_integral, floating_point
#include <utility>   // forward

#include <pugixml.hpp>

#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/misc/conversion.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

/**
 * \brief Attempts to parse an XML document from disk.
 *
 * \param path the path to the XML document.
 *
 * \return the parsed document.
 */
[[nodiscard]]
TACTILE_TMX_FORMAT_API auto load_xml_file(const FilePath& path)
    -> Result<pugi::xml_document>;

/**
 * \brief Attempts to save an XML document to disk.
 *
 * \param document    the XML document to save.
 * \param path        the destination file path.
 * \param indentation the number of spaces used for indentation.
 *
 * \return nothing on success; an error code otherwise.
 */
[[nodiscard]]
TACTILE_TMX_FORMAT_API auto save_xml_file(const pugi::xml_document& document,
                                          const FilePath& path,
                                          int indentation) -> Result<void>;

/**
 * \brief Attempts to parse an XML node attribute as a string.
 *
 * \param node the XML node that features the attribute.
 * \param key  the attribute name.
 *
 * \return the read attribute.
 */
[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_string(pugi::xml_node node, const char* key)
    -> Result<String>;

/**
 * \brief Attempts to parse an XML node attribute as a signed integer.
 *
 * \param node the XML node that features the attribute.
 * \param key  the attribute name.
 *
 * \return the read attribute.
 */
[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_int64(pugi::xml_node node, const char* key)
    -> Result<int64>;

/**
 * \brief Attempts to parse an XML node attribute as an unsigned integer.
 *
 * \param node the XML node that features the attribute.
 * \param key  the attribute name.
 *
 * \return the read attribute.
 */
[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_uint64(pugi::xml_node node, const char* key)
    -> Result<uint64>;

/**
 * \brief Attempts to parse an XML node attribute as a floating-point value.
 *
 * \param node the XML node that features the attribute.
 * \param key  the attribute name.
 *
 * \return the read attribute.
 */
[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_float64(pugi::xml_node node, const char* key)
    -> Result<float64>;

/**
 * \brief Attempts to parse an XML node attribute as a Boolean value.
 *
 * \param node the XML node that features the attribute.
 * \param key  the attribute name.
 *
 * \return the read attribute.
 */
[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_bool(pugi::xml_node node, const char* key)
    -> Result<bool>;

/**
 * \brief Attempts to parse an XML node attribute as a signed integer.
 *
 * \tparam T any signed integer type.
 *
 * \param node the XML node that features the attribute.
 * \param key  the attribute name.
 *
 * \return the read attribute.
 */
template <std::signed_integral T>
[[nodiscard]] auto parse(pugi::xml_node node, const char* key) -> Result<T>
{
  return parse_int64(node, key).and_then(
      [](const int64 i64) -> Result<T> { return narrow<T>(i64); });
}

/**
 * \brief Attempts to parse an XML node attribute as an unsigned integer.
 *
 * \tparam T any unsigned integer type.
 *
 * \param node the XML node that features the attribute.
 * \param key  the attribute name.
 *
 * \return the read attribute.
 */
template <std::unsigned_integral T>
[[nodiscard]] auto parse(pugi::xml_node node, const char* key) -> Result<T>
{
  return parse_uint64(node, key).and_then(
      [](const uint64 u64) -> Result<T> { return narrow<T>(u64); });
}

/**
 * \brief Attempts to parse an XML node attribute as a floating-point value.
 *
 * \tparam T any floating-point type.
 *
 * \param node the XML node that features the attribute.
 * \param key  the attribute name.
 *
 * \return the read attribute.
 */
template <std::floating_point T>
[[nodiscard]] auto parse(pugi::xml_node node, const char* key) -> Result<T>
{
  return parse_float64(node, key).and_then(
      [](const float64 f64) -> Result<T> { return static_cast<T>(f64); });
}

/**
 * \brief Attempts to parse an XML node attribute as a string and write it to an output parameter.
 *
 * \param      node  the XML node that features the attribute.
 * \param      key   the attribute name.
 * \param[out] value the value to write the result to.
 *
 * \return nothing on success; an error code otherwise.
 */
[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_to(pugi::xml_node node, const char* key, String& value)
    -> Result<void>;

/**
 * \brief Attempts to parse an XML node attribute as a Boolean and write it to an output parameter.
 *
 * \param      node  the XML node that features the attribute.
 * \param      key   the attribute name.
 * \param[out] value the value to write the result to.
 *
 * \return nothing on success; an error code otherwise.
 */
[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_to(pugi::xml_node node, const char* key, bool& value)
    -> Result<void>;

/**
 * \brief Attempts to parse an XML node attribute as a signed integer and write it to an output parameter.
 *
 * \param      node  the XML node that features the attribute.
 * \param      key   the attribute name.
 * \param[out] value the value to write the result to.
 *
 * \return nothing on success; an error code otherwise.
 */
template <std::signed_integral T>
[[nodiscard]] auto parse_to(pugi::xml_node node, const char* key, T& value)
    -> Result<void>
{
  return parse_int64(node, key).and_then([&](const int64 i64) {
    value = narrow<T>(i64);
    return kOK;
  });
}

/**
 * \brief Attempts to parse an XML node attribute as an unsigned integer and write it to an output parameter.
 *
 * \param      node  the XML node that features the attribute.
 * \param      key   the attribute name.
 * \param[out] value the value to write the result to.
 *
 * \return nothing on success; an error code otherwise.
 */
template <std::unsigned_integral T>
[[nodiscard]] auto parse_to(pugi::xml_node node, const char* key, T& value)
    -> Result<void>
{
  return parse_uint64(node, key).and_then([&](const uint64 u64) {
    value = narrow<T>(u64);
    return kOK;
  });
}

/**
 * \brief Attempts to parse an XML node attribute as a floating-point value and write it to an output parameter.
 *
 * \param      node  the XML node that features the attribute.
 * \param      key   the attribute name.
 * \param[out] value the value to write the result to.
 *
 * \return nothing on success; an error code otherwise.
 */
template <std::floating_point T>
[[nodiscard]] auto parse_to(pugi::xml_node node, const char* key, T& value)
    -> Result<void>
{
  return parse_float64(node, key).and_then([&](const float64 f64) {
    value = static_cast<T>(f64);
    return kOK;
  });
}

/**
 * \brief Attempts to parse an XML node attribute and write it to a value, using a default value on failure.
 *
 * \param      node          the XML node that features the attribute.
 * \param      key           the attribute name.
 * \param[out] value         the value to write the result to.
 * \param      default_value the fallback value used if the attribute didn't exist.
 *
 * \return nothing on success; an error code otherwise.
 */
template <typename T>
[[nodiscard]] auto try_parse_to(const pugi::xml_node node,
                                const char* key,
                                T& value,
                                T&& default_value = T {}) -> Result<void>
{
  if (!node.attribute(key).empty()) {
    return parse_to(node, key, value);
  }
  else {
    value = std::forward<T>(default_value);
    return kOK;
  }
}

}  // namespace tactile::tiled::tmx

#ifndef RUNE_TMX_PROPERTY_HPP
#define RUNE_TMX_PROPERTY_HPP

#include <algorithm>    // any_of, find_if
#include <cassert>      // assert
#include <concepts>     // same_as
#include <nenya.hpp>    // strong_type
#include <string>       // string
#include <string_view>  // string_view
#include <variant>      // variant, get, get_if, holds_alternative
#include <vector>       // vector

#include "../../aliases/json_type.hpp"
#include "core/rune_error.hpp"
#include "tmx_color.hpp"
#include "tmx_property_type.hpp"

namespace rune {

/// \cond FALSE
namespace tags {
struct tmx_file_tag;
struct tmx_object_id_tag;
}  // namespace tags
/// \endcond

/// \addtogroup tmx
/// \{

using tmx_file = nenya::strong_type<std::string, tags::tmx_file_tag>;
using tmx_object_id = nenya::strong_type<int, tags::tmx_object_id_tag>;

/**
 * \brief Represents a property, with an associated name and value.
 */
struct tmx_property final
{
  using data_type =
      std::variant<std::string, tmx_file, tmx_object_id, tmx_color, int, float, bool>;

  std::string name;
  tmx_property_type type{tmx_property_type::string};
  data_type value;
};

using tmx_properties = std::vector<tmx_property>;

inline void from_json(const json_type& json, tmx_property& property)
{
  json.at("name").get_to(property.name);
  json.at("type").get_to(property.type);

  switch (property.type)
  {
    default:
      assert(false && "from_json() for tmx_property has missing branch!");

    case tmx_property_type::string:
      property.value.emplace<std::string>(json.at("value").get<std::string>());
      break;

    case tmx_property_type::integer:
      property.value.emplace<int>(json.at("value").get<int>());
      break;

    case tmx_property_type::floating:
      property.value.emplace<float>(json.at("value").get<float>());
      break;

    case tmx_property_type::boolean:
      property.value.emplace<bool>(json.at("value").get<bool>());
      break;

    case tmx_property_type::color:
      property.value.emplace<tmx_color>(json.at("value").get<tmx_color>());
      break;

    case tmx_property_type::file:
      property.value.emplace<tmx_file>(json.at("value").get<std::string>());
      break;

    case tmx_property_type::object:
      property.value.emplace<tmx_object_id>(json.at("value").get<int>());
      break;
  }
}

/// \} End of group tmx

namespace tmx {

/// \addtogroup tmx
/// \{

// clang-format off

template <typename T>
concept property_value_type = std::same_as<T, int> ||
                              std::same_as<T, float> ||
                              std::same_as<T, bool> ||
                              std::same_as<T, std::string> ||
                              std::same_as<T, tmx_color> ||
                              std::same_as<T, tmx_file> ||
                              std::same_as<T, tmx_object_id>;

// clang-format on

/// \name Property functions
/// \{

[[nodiscard]] inline auto try_get(const tmx_properties& properties,
                                  const std::string_view name)
    -> tmx_properties::const_iterator
{
  return std::ranges::find_if(properties, [name](const tmx_property& property) noexcept {
    return property.name == name;
  });
}

template <property_value_type T>
[[nodiscard]] auto get_if(const tmx_properties& properties,
                          const std::string_view name) noexcept -> const T*
{
  if (const auto it = try_get(properties, name); it != properties.end())
  {
    return std::get_if<T>(&it->value);
  }
  else
  {
    return nullptr;
  }
}

[[nodiscard]] inline auto get_if_string(const tmx_property& property) noexcept
    -> const std::string*
{
  return std::get_if<std::string>(&property.value);
}

[[nodiscard]] inline auto get_if_string(const tmx_properties& properties,
                                        const std::string_view name) -> const std::string*
{
  return get_if<std::string>(properties, name);
}

[[nodiscard]] inline auto get_if_int(const tmx_property& property) noexcept -> const int*
{
  return std::get_if<int>(&property.value);
}

[[nodiscard]] inline auto get_if_int(const tmx_properties& properties,
                                     const std::string_view name) -> const int*
{
  return get_if<int>(properties, name);
}

[[nodiscard]] inline auto get_if_float(const tmx_property& property) noexcept -> const
    float*
{
  return std::get_if<float>(&property.value);
}

[[nodiscard]] inline auto get_if_float(const tmx_properties& properties,
                                       const std::string_view name) -> const float*
{
  return get_if<float>(properties, name);
}

[[nodiscard]] inline auto get_if_bool(const tmx_property& property) noexcept -> const
    bool*
{
  return std::get_if<bool>(&property.value);
}

[[nodiscard]] inline auto get_if_bool(const tmx_properties& properties,
                                      const std::string_view name) -> const bool*
{
  return get_if<bool>(properties, name);
}

[[nodiscard]] inline auto get_if_color(const tmx_property& property) noexcept
    -> const tmx_color*
{
  return std::get_if<tmx_color>(&property.value);
}

[[nodiscard]] inline auto get_if_color(const tmx_properties& properties,
                                       const std::string_view name) -> const tmx_color*
{
  return get_if<tmx_color>(properties, name);
}

[[nodiscard]] inline auto get_if_file(const tmx_property& property) noexcept
    -> const tmx_file*
{
  return std::get_if<tmx_file>(&property.value);
}

[[nodiscard]] inline auto get_if_file(const tmx_properties& properties,
                                      const std::string_view name) -> const tmx_file*
{
  return get_if<tmx_file>(properties, name);
}

[[nodiscard]] inline auto get_if_object(const tmx_property& property) noexcept
    -> const tmx_object_id*
{
  return std::get_if<tmx_object_id>(&property.value);
}

[[nodiscard]] inline auto get_if_object(const tmx_properties& properties,
                                        const std::string_view name)
    -> const tmx_object_id*
{
  return get_if<tmx_object_id>(properties, name);
}

template <property_value_type T>
[[nodiscard]] auto is(const tmx_property& property) noexcept -> bool
{
  return std::holds_alternative<T>(property.value);
}

template <property_value_type T>
[[nodiscard]] auto is(const tmx_properties& properties, const std::string_view name)
    -> bool
{
  if (const auto it = try_get(properties, name); it != properties.end())
  {
    return is<T>(*it);
  }
  else
  {
    return false;
  }
}

[[nodiscard]] inline auto is_string(const tmx_property& property) noexcept -> bool
{
  return is<std::string>(property);
}

[[nodiscard]] inline auto is_string(const tmx_properties& properties,
                                    const std::string_view name) -> bool
{
  return is<std::string>(properties, name);
}

[[nodiscard]] inline auto is_int(const tmx_property& property) noexcept -> bool
{
  return is<int>(property);
}

[[nodiscard]] inline auto is_int(const tmx_properties& properties,
                                 const std::string_view name) -> bool
{
  return is<int>(properties, name);
}

[[nodiscard]] inline auto is_float(const tmx_property& property) noexcept -> bool
{
  return is<float>(property);
}

[[nodiscard]] inline auto is_float(const tmx_properties& properties,
                                   const std::string_view name) -> bool
{
  return is<float>(properties, name);
}

[[nodiscard]] inline auto is_bool(const tmx_property& property) noexcept -> bool
{
  return is<bool>(property);
}

[[nodiscard]] inline auto is_bool(const tmx_properties& properties,
                                  const std::string_view name) -> bool
{
  return is<bool>(properties, name);
}

[[nodiscard]] inline auto is_color(const tmx_property& property) noexcept -> bool
{
  return is<tmx_color>(property);
}

[[nodiscard]] inline auto is_color(const tmx_properties& properties,
                                   const std::string_view name) -> bool
{
  return is<tmx_color>(properties, name);
}

[[nodiscard]] inline auto is_file(const tmx_property& property) noexcept -> bool
{
  return is<tmx_file>(property);
}

[[nodiscard]] inline auto is_file(const tmx_properties& properties,
                                  const std::string_view name) -> bool
{
  return is<tmx_file>(properties, name);
}

[[nodiscard]] inline auto is_object(const tmx_property& property) noexcept -> bool
{
  return is<tmx_object_id>(property);
}

[[nodiscard]] inline auto is_object(const tmx_properties& properties,
                                    const std::string_view name) -> bool
{
  return is<tmx_object_id>(properties, name);
}

template <property_value_type T>
[[nodiscard]] auto get(const tmx_property& property) -> const T&
{
  return std::get<T>(property.value);
}

template <property_value_type T>
[[nodiscard]] auto get(const tmx_properties& properties, const std::string_view name)
    -> const T&
{
  if (const auto it = try_get(properties, name); it != properties.end())
  {
    return get<T>(*it);
  }
  else
  {
    throw rune_error{"Could not find property with the specified name!"};
  }
}

[[nodiscard]] inline auto get_string(const tmx_property& property) -> const std::string&
{
  return get<std::string>(property);
}

[[nodiscard]] inline auto get_string(const tmx_properties& properties,
                                     const std::string_view name) -> const std::string&
{
  return get<std::string>(properties, name);
}

[[nodiscard]] inline auto get_int(const tmx_property& property) -> int
{
  return get<int>(property);
}

[[nodiscard]] inline auto get_int(const tmx_properties& properties,
                                  const std::string_view name) -> int
{
  return get<int>(properties, name);
}

[[nodiscard]] inline auto get_float(const tmx_property& property) -> float
{
  return get<float>(property);
}

[[nodiscard]] inline auto get_float(const tmx_properties& properties,
                                    const std::string_view name) -> float
{
  return get<float>(properties, name);
}

[[nodiscard]] inline auto get_bool(const tmx_property& property) -> bool
{
  return get<bool>(property);
}

[[nodiscard]] inline auto get_bool(const tmx_properties& properties,
                                   const std::string_view name) -> bool
{
  return get<bool>(properties, name);
}

[[nodiscard]] inline auto get_color(const tmx_property& property) -> const tmx_color&
{
  return get<tmx_color>(property);
}

[[nodiscard]] inline auto get_color(const tmx_properties& properties,
                                    const std::string_view name) -> const tmx_color&
{
  return get<tmx_color>(properties, name);
}

[[nodiscard]] inline auto get_file(const tmx_property& property) -> const tmx_file&
{
  return get<tmx_file>(property);
}

[[nodiscard]] inline auto get_file(const tmx_properties& properties,
                                   const std::string_view name) -> const tmx_file&
{
  return get<tmx_file>(properties, name);
}

[[nodiscard]] inline auto get_object(const tmx_property& property) -> tmx_object_id
{
  return get<tmx_object_id>(property);
}

[[nodiscard]] inline auto get_object(const tmx_properties& properties,
                                     const std::string_view name) -> tmx_object_id
{
  return get<tmx_object_id>(properties, name);
}

/**
 * \brief Indicates whether or not a property with the specified name exists in a vector
 * of properties.
 *
 * \param properties the vector of properties that will be searched.
 * \param name the name of the property to look for.
 *
 * \return `true` if the properties contains a property with the specified name; `false`
 * otherwise.
 */
[[nodiscard]] inline auto contains(const tmx_properties& properties,
                                   const std::string_view name) -> bool
{
  return std::ranges::any_of(properties, [name](const tmx_property& property) noexcept {
    return property.name == name;
  });
}

/**
 * \brief Attempts to find and return a property with the specified name.
 *
 * \param properties the properties that will be searched.
 * \param name the name of the desired property.
 *
 * \return the property with the specified name.
 *
 * \throws rune_error if there is no property with the specified name.
 */
[[nodiscard]] inline auto at(const tmx_properties& properties,
                             const std::string_view name) -> const tmx_property&
{
  if (const auto it = try_get(properties, name); it != properties.end())
  {
    return *it;
  }
  else
  {
    throw rune_error{"Could not find property with specified name!"};
  }
}

/// \} End of property functions

/// \} End of group tmx

}  // namespace tmx

}  // namespace rune

#endif  // RUNE_TMX_PROPERTY_HPP

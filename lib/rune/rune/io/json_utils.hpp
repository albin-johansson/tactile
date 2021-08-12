#ifndef RUNE_IO_JSON_UTILS_HPP
#define RUNE_IO_JSON_UTILS_HPP

#include <cassert>      // assert
#include <concepts>     // same_as
#include <filesystem>   // path
#include <fstream>      // ifstream
#include <string_view>  // string_view
#include <variant>      // variant
#include <vector>       // vector

#include "../aliases/json_type.hpp"
#include "../aliases/maybe.hpp"
#include "../core/rune_error.hpp"

namespace rune {

/// \addtogroup io
/// \{

/// \name JSON
/// \{

// clang-format off

template <typename T>
concept json_serializable_type = requires (json_type json)
{
  { json.get<T>() };
};

// clang-format on

/**
 * \brief Parses the JSON file at the specified path, and returns the contents.
 *
 * \pre `file` must refer to a JSON file.
 *
 * \param file the file path of the JSON file.
 *
 * \return the parsed JSON data.
 */
[[nodiscard]] inline auto read_json(const std::filesystem::path& file) -> json_type
{
  assert(file.extension() == ".json");
  std::ifstream stream{file};

  json_type json;
  stream >> json;

  return json;
}

/// \} End of JSON

/// \} End of group io

namespace io {

/// \addtogroup io
/// \{

/// \name JSON
/// \{

/**
 * \brief Fills a vector with values from a JSON array.
 *
 * \pre `array` must represent a JSON array.
 *
 * \param array the JSON array that provides the source data.
 * \param[out] container the vector that will be filled.
 *
 * \see `try_get_to()`
 *
 * \since 0.1.0
 */
template <json_serializable_type T>
void get_to(const json_type& array, std::vector<T>& container)
{
  assert(array.is_array());

  container.reserve(array.size());
  for (const auto& [key, value] : array.items())
  {
    container.push_back(value.template get<T>());
  }
}

/**
 * \brief Fills a vector with values from a JSON array in a JSON object.
 *
 * \pre The JSON element associated with the specified key must be an array.
 *
 * \param json the JSON object that contains an array associated with the specified key.
 * \param key the key of the child JSON array element.
 * \param[out] container the vector that will be filled.
 *
 * \see `try_get_to()`
 *
 * \throws rune_error if there is no key with the specified name.
 *
 * \since 0.1.0
 */
template <json_serializable_type T>
void get_to(const json_type& json, const std::string_view key, std::vector<T>& container)
{
  const auto it = json.find(key);
  if (it == json.end())
  {
    throw rune_error{"io::get_to(): key does not exist"};
  }

  assert(it->is_array());
  container.reserve(it->size());
  for (const auto& [key, value] : it->items())
  {
    container.push_back(value.template get<T>());
  }
}

/**
 * \brief Attempts to serialize and assign a value.
 *
 * \details This function has no effect if there is no element associated with the
 * specified key.
 *
 * \tparam T the type of the serializable value.
 *
 * \param json the source JSON object.
 * \param key the key associated with the desired JSON value.
 * \param[out] value the value that will be assigned.
 *
 * \see `get_to()`
 */
template <json_serializable_type T>
void try_get_to(const json_type& json, const std::string_view key, T& value)
{
  if (const auto it = json.find(key); it != json.end())
  {
    value = it->get<T>();
  }
}

/// \copydoc try_get_to()
template <json_serializable_type T>
void try_get_to(const json_type& json, const std::string_view key, maybe<T>& value)
{
  if (const auto it = json.find(key); it != json.end())
  {
    value = it->get<T>();
  }
}

/**
 * \brief Fills a vector with values from a JSON array, if it exists.
 *
 * \note This function has no effect if there is no element associated with the specified
 * key.
 *
 * \pre The JSON element associated with the specified key must be an array.
 *
 * \param json the JSON object that contains an array associated with the specified key.
 * \param key the key of the JSON array element.
 * \param[out] container the vector that will be filled.
 *
 * \see `get_to()`
 *
 * \since 0.1.0
 */
template <json_serializable_type T>
void try_get_to(const json_type& json,
                const std::string_view key,
                std::vector<T>& container)
{
  if (const auto it = json.find(key); it != json.end())
  {
    assert(it->is_array());
    container.reserve(it->size());
    for (const auto& [key, value] : it->items())
    {
      container.push_back(value.template get<T>());
    }
  }
}

/**
 * \brief Assigns a wrapper from a serializable value.
 *
 * \tparam T the type of the wrapper.
 *
 * \param json the source JSON object.
 * \param key the key associated with the desired JSON value.
 * \param[out] value the value that will be assigned.
 *
 * \throws rune_error if there is no key with the specified name.
 *
 * \see `try_emplace_to()`
 *
 * \since 0.1.0
 */
template <typename T>
  requires json_serializable_type<typename T::value_type>
void emplace_to(const json_type& json, const std::string_view key, T& value)
{
  if (const auto it = json.find(key); it != json.end())
  {
    value = T{it->get<typename T::value_type>()};
  }
  else
  {
    throw rune_error{"io::emplace_to(): key does not exist"};
  }
}

/// \copydoc emplace_to()
template <typename T>
  requires json_serializable_type<typename T::value_type>
void emplace_to(const json_type& json, const std::string_view key, maybe<T>& value)
{
  if (const auto it = json.find(key); it != json.end())
  {
    value = T{it->get<typename T::value_type>()};
  }
  else
  {
    throw rune_error{"io::emplace_to(): key does not exist"};
  }
}

/**
 * \brief Attempts to assign a wrapper from a serializable value.
 *
 * \details This function has no effect if there is no element associated with the
 * specified key.
 *
 * \tparam T the type of the wrapper.
 *
 * \param json the source JSON object.
 * \param key the key associated with the desired JSON value.
 * \param[out] value the value that will be assigned.
 *
 * \see `emplace_to()`
 *
 * \since 0.1.0
 */
template <json_serializable_type T, typename... Types>
void try_emplace_to(const json_type& json,
                    const std::string_view key,
                    std::variant<Types...>& value)
{
  static_assert((std::same_as<T, Types> || ...),
                "Cannot emplace value of type that is not used by the variant!");

  if (const auto it = json.find(key); it != json.end())
  {
    value.template emplace<T>(it->get<T>());
  }
}

/// \} End of JSON

/// \} End of group io

}  // namespace io

}  // namespace rune

#endif  // RUNE_IO_JSON_UTILS_HPP

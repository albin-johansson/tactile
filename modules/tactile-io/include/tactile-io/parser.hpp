#pragma once

#include <filesystem>  // path

#include "api.hpp"
#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

/**
 * \brief Attempts to parse a Tiled JSON map at the specified file path.
 *
 * \note Do not call this function directly, use `ParseMap()` instead.
 *
 * \param path the path to the JSON file that will be parsed.
 * \param[out] error optional pointer to which parse errors are reported.
 *
 * \return an owning pointer (which must be freed using `DeleteMap()`!) to the parsed
 * intermediate map representation; a null pointer is returned if something goes wrong
 * whilst parsing the map.
 *
 * \see `ParseMap()`
 */
TACTILE_IO_API_QUERY
auto ParseJsonMap(CPathStr path, ParseError* error = nullptr) -> Map*;

/**
 * \brief Attempts to parse a Tiled XML (TMX) map at the specified file path.
 *
 * \note Do not call this function directly, use `ParseMap()` instead.
 *
 * \param path the path to the XML file that will be parsed.
 * \param[out] error optional pointer to which parse errors are reported.
 *
 * \return an owning pointer (which must be freed using `DeleteMap()`!) to the parsed
 * intermediate map representation; a null pointer is returned if something goes wrong
 * whilst parsing the map.
 *
 * \see `ParseMap()`
 */
TACTILE_IO_API_QUERY
auto ParseXmlMap(CPathStr path, ParseError* error = nullptr) -> Map*;

/**
 * \brief Attempts to parse a Tactile YAML map at the specified file path.
 *
 * \note Do not call this function directly, use `ParseMap()` instead.
 *
 * \param path the path to the YAML file that will be parsed.
 * \param[out] error optional pointer to which parse errors are reported.
 *
 * \return an owning pointer (which must be freed using `DeleteMap()`!) to the parsed
 * intermediate map representation; a null pointer is returned if something goes wrong
 * whilst parsing the map.
 *
 * \see `ParseMap()`
 */
TACTILE_IO_API_QUERY
auto ParseYamlMap(CPathStr path, ParseError* error = nullptr) -> Map*;

/**
 * \brief Attempts to parse a map at the specified file path.
 *
 * \details This inline function is provided as a convenience that automatically deduces
 * which kind of map to parse. You do not have to worry about memory management with this
 * function, since it returns a unique pointer.
 *
 * \details This function can be used to parse both the Tactile map format and the Tiled
 * JSON and XML formats. Attributes not used in Tactile are simply ignored in Tiled map
 * files. However, certain Tiled constructs are not supported, such as infinite maps,
 * which would cause a parse error being reported.
 *
 * \details Supported file extensions include `.yaml`, `.yml`, `.json`, `.xml` and `.tmx`.
 *
 * \param path the path to the file that will be parsed.
 * \param[out] error optional pointer to which parse errors are reported.
 *
 * \return a unique pointer to the parsed intermediate map representation; a null pointer
 * is returned if something goes wrong whilst parsing the map.
 *
 * \see `ParseJsonMap()`
 * \see `ParseXmlMap()`
 * \see `ParseYamlMap()`
 */
[[nodiscard]] inline auto ParseMap(const std::filesystem::path& path,
                                   ParseError* error = nullptr) -> MapPtr
{
  const auto ext = path.extension();
  if (ext == ".yaml" || ext == ".yml") {
    return MapPtr{ParseYamlMap(path.c_str(), error)};
  }
  else if (ext == ".json") {
    return MapPtr{ParseJsonMap(path.c_str(), error)};
  }
  else if (ext == ".xml" || ext == ".tmx") {
    return MapPtr{ParseXmlMap(path.c_str(), error)};
  }
  else {
    if (error) {
      *error = ParseError::MapUnsupportedExtension;
    }

    return nullptr;
  }
}

}  // namespace Tactile::IO
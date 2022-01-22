#ifndef TACTILE_IO_IR_COMMON_HPP_
#define TACTILE_IO_IR_COMMON_HPP_

#include <cstddef>     // size_t
#include <cstdint>     // uint8_t, int32_t
#include <filesystem>  // path

#include <tactile_def.hpp>

#include "api.hpp"

namespace Tactile::IO {

/* Forward declarations of IR types, which are not defined in client code. */
struct Property;
struct Component;
struct ComponentDef;
struct AnimationFrame;
struct Object;
struct Layer;
struct TileLayer;
struct ObjectLayer;
struct GroupLayer;
struct Tile;
struct Tileset;
struct Map;

/// \brief Alias for a C-style string using the native preferred path `char` type.
using CPathStr = const std::filesystem::path::value_type*;

/// \brief A simple representation of an 8-bit RGBA color.
struct Color final
{
  uint8 red{};
  uint8 green{};
  uint8 blue{};
  uint8 alpha{};
};

/// \brief Represents different map formats.
enum class MapFormat {
  Unsupported,  ///< Unsupported/invalid/unknown map format.
  Yaml,         ///< Tactile YAML map format.
  Json,         ///< Tiled JSON map format.
  Xml           ///< Tiled XML (TMX) map format.
};

/**
 * \brief Attempts to deduce the map format to be used based on a file path string.
 *
 * \details This function essentially checks the extension of the supplied string and
 * tries to map the extension to one of the YAML, JSON or XML map formats.
 *
 * \param str the file path string that will be checked.
 *
 * \return the deduced map format; `MapFormat::Unsupported` is returned for either
 * unsupported or unrecognized formats.
 */
TACTILE_IO_API_QUERY
auto DeduceFormat(CPathStr str) -> MapFormat;

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_COMMON_HPP_

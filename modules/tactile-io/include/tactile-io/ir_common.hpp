#ifndef TACTILE_IO_IR_COMMON_HPP_
#define TACTILE_IO_IR_COMMON_HPP_

#include <cstddef>     // size_t
#include <cstdint>     // uint8_t, int32_t
#include <filesystem>  // path::value_type

namespace Tactile::IO {

/* Forward declarations of IR types, which are not defined in client code. */
struct Property;
struct AnimationFrame;
struct Object;
struct Layer;
struct TileLayer;
struct ObjectLayer;
struct GroupLayer;
struct Tile;
struct Tileset;
struct Map;

using usize = std::size_t;   ///< Large unsigned integer.
using uint8 = std::uint8_t;  ///< 8-bit unsigned integer.
using int32 = std::int32_t;  ///< 32-bit signed integer.

/// \brief Alias for a C-style string.
using CStr = const char*;

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

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_COMMON_HPP_

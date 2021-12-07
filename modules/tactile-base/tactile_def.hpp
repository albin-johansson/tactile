#ifndef TACTILE_TACTILE_HPP_
#define TACTILE_TACTILE_HPP_

#include <cstddef>      // size_t
#include <cstdint>      // int{}_t, uint{}_t
#include <functional>   // reference_wrapper
#include <optional>     // optional, nullopt_t, nullopt
#include <type_traits>  // is_pointer_v

namespace Tactile {

using CStr = const char*;

using uchar = unsigned char;
using uint = unsigned int;
using ulonglong = unsigned long long int;
using usize = std::size_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

// General ID used to identify different property contexts (not persistent)
using ContextID = int32;

using MapID = int32;
using TilesetID = int32;
using LayerID = int32;
using ObjectID = int32;
using TileID = int32;

constexpr TileID empty_tile = 0;

using TileRow = std::vector<TileID>;
using TileMatrix = std::vector<TileRow>;

template <typename T>
  requires std::is_pointer_v<T>
using NotNull = T;

template <typename T>
using Ref = std::reference_wrapper<T>;

template <typename T>
using Maybe = std::optional<T>;

constexpr std::nullopt_t nothing = std::nullopt;

/// \brief Provides values for all different possible property types.
enum class PropertyType {
  String,    ///< A string property.
  Integer,   ///< An integer property.
  Floating,  ///< A floating-point property.
  Boolean,   ///< A boolean property.
  File,      ///< A file path property.
  Color,     ///< A color property.
  Object     ///< An integer ID property, that refers to a map object.
};

/// \brief Represents the different available map object types.
enum class ObjectType {
  Point,
  Rectangle,
  Ellipse
};

/// \brief Represents all different layer types.
enum class LayerType {
  TileLayer,
  ObjectLayer,
  GroupLayer
};

}  // namespace Tactile

#endif  // TACTILE_TACTILE_HPP_

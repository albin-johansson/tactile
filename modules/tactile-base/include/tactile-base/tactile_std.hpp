#pragma once

#include <cstddef>      // size_t
#include <cstdint>      // uint{}_t, int{}_t
#include <functional>   // reference_wrapper
#include <optional>     // optional
#include <type_traits>  // is_pointer_v
#include <vector>       // vector

#include <nenya.hpp>  // strong_type

namespace Tactile {

template <typename T>
  requires std::is_pointer_v<T>
using NotNull = T;

template <typename T>
using Ref = std::reference_wrapper<T>;

template <typename T>
using Maybe = std::optional<T>;

constexpr std::nullopt_t nothing = std::nullopt;

using CStr = const char*;

using usize = std::size_t;
using uchar = unsigned char;
using uint = unsigned int;
using ulonglong = unsigned long long;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

namespace Tags {
struct ObjectRef;
struct ContextID;
struct ObjectID;
struct TileID;
struct LayerID;
struct MapID;
struct TilesetID;
}  // namespace Tags

// General ID used to identify different property contexts (these IDs are not saved)
using ContextID = nenya::strong_type<int32, Tags::ContextID>;

using ObjectRef = nenya::strong_type<int32, Tags::ObjectRef>;
using ObjectID = nenya::strong_type<int32, Tags::ObjectID>;
using TileID = nenya::strong_type<int32, Tags::TileID>;
using LayerID = nenya::strong_type<int32, Tags::LayerID>;
using MapID = nenya::strong_type<int32, Tags::MapID>;
using TilesetID = nenya::strong_type<int32, Tags::TilesetID>;

constexpr TileID empty_tile{0};

using TileRow = std::vector<TileID>;
using TileMatrix = std::vector<TileRow>;

[[nodiscard]] constexpr auto operator""_layer(const ulonglong value) noexcept -> LayerID
{
  return LayerID{static_cast<LayerID::value_type>(value)};
}

[[nodiscard]] constexpr auto operator""_obj(const ulonglong value) noexcept -> ObjectID
{
  return ObjectID{static_cast<ObjectID::value_type>(value)};
}

[[nodiscard]] constexpr auto operator""_tile(const ulonglong value) noexcept -> TileID
{
  return TileID{static_cast<TileID::value_type>(value)};
}

[[nodiscard]] constexpr auto operator""_tset(const ulonglong value) noexcept -> TilesetID
{
  return TilesetID{static_cast<TilesetID::value_type>(value)};
}

[[nodiscard]] inline auto MakeTileRow(const usize nCols, const TileID value) -> TileRow
{
  TileRow row;
  row.reserve(nCols);
  row.assign(nCols, value);
  return row;
}

[[nodiscard]] inline auto MakeTileMatrix(const usize nRows, const usize nCols)
    -> TileMatrix
{
  TileMatrix tiles;
  tiles.reserve(nRows);
  tiles.assign(nRows, MakeTileRow(nCols, empty_tile));
  return tiles;
}

}  // namespace Tactile

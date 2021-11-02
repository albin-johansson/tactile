#pragma once

#include <concepts>  // invocable
#include <memory>    // unique_ptr

#include <tactile-base/layer_type.hpp>
#include <tactile-base/object_type.hpp>
#include <tactile-base/property_type.hpp>

#include "api.hpp"
#include "ir_common.hpp"
#include "ir_map.hpp"
#include "ir_object.hpp"
#include "ir_property.hpp"
#include "ir_tile.hpp"
#include "ir_tileset.hpp"

namespace Tactile::IO {

/// \addtogroup io
/// \{

/// \name Animation API
/// \{

TACTILE_IO_API void SetTile(AnimationFrame& frame, int32 tile);
TACTILE_IO_API void SetDuration(AnimationFrame& frame, int32 ms);

TACTILE_IO_API_QUERY auto GetTile(const AnimationFrame& frame) -> int32;
TACTILE_IO_API_QUERY auto GetDuration(const AnimationFrame& frame) -> int32;

/// \} End of animation API

/// \name Layer API
/// \{

TACTILE_IO_API_QUERY auto MarkAsTileLayer(Layer& layer) -> TileLayer&;
TACTILE_IO_API_QUERY auto MarkAsObjectLayer(Layer& layer) -> ObjectLayer&;
TACTILE_IO_API_QUERY auto MarkAsGroupLayer(Layer& layer) -> GroupLayer&;

TACTILE_IO_API_QUERY auto GetTileLayer(const Layer& layer) -> const TileLayer&;
TACTILE_IO_API_QUERY auto GetObjectLayer(const Layer& layer) -> const ObjectLayer&;
TACTILE_IO_API_QUERY auto GetGroupLayer(const Layer& layer) -> const GroupLayer&;

TACTILE_IO_API void SetId(Layer& layer, int32 id);
TACTILE_IO_API void SetIndex(Layer& layer, usize index);
TACTILE_IO_API void SetType(Layer& layer, LayerType type);
TACTILE_IO_API void SetName(Layer& layer, CStr name);
TACTILE_IO_API void SetOpacity(Layer& layer, float opacity);
TACTILE_IO_API void SetVisible(Layer& layer, bool visible);

TACTILE_IO_API_QUERY auto GetId(const Layer& layer) -> int32;
TACTILE_IO_API_QUERY auto GetIndex(const Layer& layer) -> usize;
TACTILE_IO_API_QUERY auto GetType(const Layer& layer) -> LayerType;
TACTILE_IO_API_QUERY auto GetName(const Layer& layer) -> CStr;
TACTILE_IO_API_QUERY auto GetOpacity(const Layer& layer) -> float;
TACTILE_IO_API_QUERY auto IsVisible(const Layer& layer) -> bool;

TACTILE_IO_API_QUERY auto IsTileLayer(const Layer& layer) -> bool;
TACTILE_IO_API_QUERY auto IsObjectLayer(const Layer& layer) -> bool;
TACTILE_IO_API_QUERY auto IsGroupLayer(const Layer& layer) -> bool;

TACTILE_IO_API_QUERY auto GetPropertyCount(const Layer& layer) -> usize;
TACTILE_IO_API_QUERY auto GetProperty(const Layer& layer, usize index) -> const Property&;

TACTILE_IO_API void ReserveProperties(Layer& layer, usize n);
TACTILE_IO_API_QUERY auto AddProperty(Layer& layer) -> Property&;

/// \} End of layer API

/// \name Tile layer API
/// \{

TACTILE_IO_API void ReserveTiles(TileLayer& layer, int32 nRows, int32 nCols);
TACTILE_IO_API void SetTile(TileLayer& layer, int32 row, int32 column, int32 tile);

TACTILE_IO_API_QUERY auto GetRowCount(const TileLayer& layer) -> int32;
TACTILE_IO_API_QUERY auto GetColumnCount(const TileLayer& layer) -> int32;
TACTILE_IO_API_QUERY auto GetTile(const TileLayer& layer, int32 row, int32 column)
    -> int32;

/// \} End of tile layer API

/// \name Object layer API
/// \{

TACTILE_IO_API void ReserveObjects(ObjectLayer& layer, usize n);
TACTILE_IO_API_QUERY auto AddObject(ObjectLayer& layer) -> Object&;

TACTILE_IO_API_QUERY auto GetObjectCount(const ObjectLayer& layer) -> usize;
TACTILE_IO_API_QUERY auto GetObject(const ObjectLayer& layer, usize index)
    -> const Object&;

/// \} End of object layer API

/// \name Group layer API
/// \{

TACTILE_IO_API void ReserveLayers(GroupLayer& layer, usize n);
TACTILE_IO_API_QUERY auto AddLayer(GroupLayer& layer) -> Layer&;

TACTILE_IO_API_QUERY auto GetLayerCount(const GroupLayer& layer) -> usize;
TACTILE_IO_API_QUERY auto GetLayer(const GroupLayer& layer, usize index) -> const Layer&;

/// \} End of group layer API

/**
 * \brief Calls the supplied callable for each tileset provided by the map.
 *
 * \tparam U the type of the function object.
 *
 * \param map the map that provides the layers to be visited.
 * \param callable the function object that is invoked for each layer.
 */
template <std::invocable<const Tileset&> U>
void EachTileset(const Map& map, U&& callable)
{
  const auto count = GetTilesetCount(map);
  for (usize index = 0; index < count; ++index) {
    const auto& tileset = GetTileset(map, index);
    callable(tileset);
  }
}

/**
 * \brief Calls the supplied callable for each tile info object provided by the tileset.
 *
 * \note Do not confuse the notion of tile info objects and the individual tiles
 * associated with a tileset.
 *
 * \tparam U the type of the function object.
 *
 * \param tileset the tileset that provides the tile info objects to be visited.
 * \param callable the function object that is invoked for each tile info.
 */
template <std::invocable<const Tile&> U>
void EachTileInfo(const Tileset& tileset, U&& callable)
{
  const auto count = GetTileInfoCount(tileset);
  for (usize index = 0; index < count; ++index) {
    const auto& tile = GetTileInfo(tileset, index);
    callable(tile);
  }
}

template <std::invocable<const AnimationFrame&> U>
void EachAnimationFrame(const Tile& tile, U&& callable)
{
  const auto count = GetAnimationFrameCount(tile);
  for (usize index = 0; index < count; ++index) {
    const auto& frame = GetAnimationFrame(tile, index);
    callable(frame);
  }
}

/**
 * \brief Calls the supplied callable for each layer provided by the source object.
 *
 * \tparam T the type of the source object.
 * \tparam U the type of the function object.
 *
 * \param source the source object that provides the layers to be visited.
 * \param callable the function object that is invoked for each layer.
 */
template <typename T, std::invocable<const Layer&> U>
void EachLayer(const T& source, U&& callable)
{
  const auto count = GetLayerCount(source);
  for (usize index = 0; index < count; ++index) {
    const auto& layer = GetLayer(source, index);
    callable(layer);
  }
}

/**
 * \brief Calls the supplied callable for each map object provided by the source object.
 *
 * \tparam T the type of the source object.
 * \tparam U the type of the function object.
 *
 * \param source the source object that provides the map objects to be visited.
 * \param callable the function object that is invoked for each map object.
 */
template <typename T, std::invocable<const Object&> U>
void EachObject(const T& source, U&& callable)
{
  const auto count = GetObjectCount(source);
  for (usize index = 0; index < count; ++index) {
    const auto& object = GetObject(source, index);
    callable(object);
  }
}

/**
 * \brief Calls the supplied callable for each property provided by the source object.
 *
 * \tparam T the type of the source object.
 * \tparam U the type of the function object.
 *
 * \param source the source object that provides the properties to be visited.
 * \param callable the function object that is invoked for each property.
 */
template <typename T, std::invocable<const Property&> U>
void EachProperty(const T& source, U&& callable)
{
  const auto count = GetPropertyCount(source);
  for (usize index = 0; index < count; ++index) {
    const auto& property = GetProperty(source, index);
    callable(property);
  }
}

/// \} End of group io

}  // namespace Tactile::IO

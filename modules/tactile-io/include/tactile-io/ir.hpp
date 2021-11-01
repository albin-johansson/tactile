#pragma once

#include <concepts>  // invocable
#include <memory>    // unique_ptr

#include <tactile-base/layer_type.hpp>
#include <tactile-base/object_type.hpp>
#include <tactile-base/property_type.hpp>

#include "api.hpp"
#include "ir_common.hpp"
#include "ir_map.hpp"
#include "ir_tileset.hpp"

namespace Tactile::IO {

/// \addtogroup io
/// \{

/// \name Tile API
/// \{

TACTILE_IO_API void SetId(Tile& tile, int32 id);

TACTILE_IO_API void ReserveAnimationFrames(Tile& tile, usize n);
TACTILE_IO_API void ReserveObjects(Tile& tile, usize n);
TACTILE_IO_API void ReserveProperties(Tile& tile, usize n);

TACTILE_IO_API_QUERY auto AddAnimationFrame(Tile& tile) -> AnimationFrame&;
TACTILE_IO_API_QUERY auto AddObject(Tile& tile) -> Object&;
TACTILE_IO_API_QUERY auto AddProperty(Tile& tile) -> Property&;

TACTILE_IO_API_QUERY auto IsTileWorthSaving(const Tile& tile) -> bool;
TACTILE_IO_API_QUERY auto GetId(const Tile& tile) -> int32;

TACTILE_IO_API_QUERY auto GetAnimationFrameCount(const Tile& tile) -> usize;
TACTILE_IO_API_QUERY auto GetObjectCount(const Tile& tile) -> usize;
TACTILE_IO_API_QUERY auto GetPropertyCount(const Tile& tile) -> usize;

TACTILE_IO_API_QUERY auto GetAnimationFrame(const Tile& tile, usize index)
    -> const AnimationFrame&;
TACTILE_IO_API_QUERY auto GetObject(const Tile& tile, usize index) -> const Object&;
TACTILE_IO_API_QUERY auto GetProperty(const Tile& tile, usize index) -> const Property&;

/// \} End of tile API

/// \name Animation API
/// \{

TACTILE_IO_API void SetTile(AnimationFrame& frame, int32 tile);
TACTILE_IO_API void SetDuration(AnimationFrame& frame, int32 ms);

TACTILE_IO_API_QUERY auto GetTile(const AnimationFrame& frame) -> int32;
TACTILE_IO_API_QUERY auto GetDuration(const AnimationFrame& frame) -> int32;

/// \} End of animation API

/// \name Object API
/// \{

TACTILE_IO_API void SetName(Object& object, CStr name);
TACTILE_IO_API void SetId(Object& object, int32 id);
TACTILE_IO_API void SetX(Object& object, float x);
TACTILE_IO_API void SetY(Object& object, float y);
TACTILE_IO_API void SetWidth(Object& object, float width);
TACTILE_IO_API void SetHeight(Object& object, float height);
TACTILE_IO_API void SetType(Object& object, ObjectType type);
TACTILE_IO_API void SetTag(Object& object, CStr tag);
TACTILE_IO_API void SetVisible(Object& object, bool visible);

TACTILE_IO_API void ReserveProperties(Object& object, usize n);
TACTILE_IO_API_QUERY auto AddProperty(Object& object) -> Property&;

TACTILE_IO_API_QUERY auto GetPropertyCount(const Object& object) -> usize;
TACTILE_IO_API_QUERY auto GetProperty(const Object& object, usize index)
    -> const Property&;

TACTILE_IO_API_QUERY auto GetName(const Object& object) -> CStr;
TACTILE_IO_API_QUERY auto GetId(const Object& object) -> int32;
TACTILE_IO_API_QUERY auto GetX(const Object& object) -> float;
TACTILE_IO_API_QUERY auto GetY(const Object& object) -> float;
TACTILE_IO_API_QUERY auto GetWidth(const Object& object) -> float;
TACTILE_IO_API_QUERY auto GetHeight(const Object& object) -> float;
TACTILE_IO_API_QUERY auto GetType(const Object& object) -> ObjectType;
TACTILE_IO_API_QUERY auto GetTag(const Object& object) -> CStr;
TACTILE_IO_API_QUERY auto IsVisible(const Object& object) -> bool;

/// \} End of Object API

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

/// \name Property API
/// \{

TACTILE_IO_API void SetName(Property& property, CStr name);
TACTILE_IO_API void AssignInt(Property& property, int32 value);
TACTILE_IO_API void AssignFloat(Property& property, float value);
TACTILE_IO_API void AssignString(Property& property, CStr str);
TACTILE_IO_API void AssignBool(Property& property, bool value);
TACTILE_IO_API void AssignFile(Property& property, CPathStr path);
TACTILE_IO_API void AssignObject(Property& property, int32 id);
TACTILE_IO_API void AssignColor(Property& property, const Color& color);

TACTILE_IO_API_QUERY auto GetName(const Property& property) -> CStr;
TACTILE_IO_API_QUERY auto GetType(const Property& property) -> PropertyType;
TACTILE_IO_API_QUERY auto GetInt(const Property& property) -> int32;
TACTILE_IO_API_QUERY auto GetFloat(const Property& property) -> float;
TACTILE_IO_API_QUERY auto GetString(const Property& property) -> CStr;
TACTILE_IO_API_QUERY auto GetBool(const Property& property) -> bool;
TACTILE_IO_API_QUERY auto GetFile(const Property& property) -> CPathStr;
TACTILE_IO_API_QUERY auto GetObject(const Property& property) -> int32;
TACTILE_IO_API_QUERY auto GetColor(const Property& property) -> Color;

/// \} End of property API

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

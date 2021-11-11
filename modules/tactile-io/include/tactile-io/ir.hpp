#pragma once

#include <concepts>  // invocable
#include <memory>    // unique_ptr

#include <tactile_def.hpp>

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

TACTILE_IO_API void ReserveTiles(TileLayer& layer, usize nRows, usize nCols);
TACTILE_IO_API void SetTile(TileLayer& layer, usize row, usize column, int32 tile);

TACTILE_IO_API_QUERY auto GetRowCount(const TileLayer& layer) -> usize;
TACTILE_IO_API_QUERY auto GetColumnCount(const TileLayer& layer) -> usize;
TACTILE_IO_API_QUERY auto GetTile(const TileLayer& layer, usize row, usize column)
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

/// \} End of group io

}  // namespace Tactile::IO

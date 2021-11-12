#ifndef TACTILE_IO_IR_LAYER_HPP_
#define TACTILE_IO_IR_LAYER_HPP_

#include <tactile_def.hpp>

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/**
 * \brief Marks a layer as a tile layer.
 *
 * \details Layers are considered tile layers by default.
 *
 * \param layer the layer that will be modified.
 *
 * \return the tile layer data.
 */
TACTILE_IO_API_QUERY
auto MarkAsTileLayer(Layer& layer) -> TileLayer&;

/**
 * \brief Marks a layer as an object layer.
 *
 * \param layer the layer that will be modified.
 *
 * \return the object layer data.
 */
TACTILE_IO_API_QUERY
auto MarkAsObjectLayer(Layer& layer) -> ObjectLayer&;

/**
 * \brief Marks a layer as a group layer.
 *
 * \param layer the layer that will be modified.
 *
 * \return the group layer data.
 */
TACTILE_IO_API_QUERY
auto MarkAsGroupLayer(Layer& layer) -> GroupLayer&;

/**
 * \brief Returns the tile layer data associated with a layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return the tile layer data.
 *
 * \throws TactileError if the layer isn't a tile layer.
 *
 * \see `MarkAsTileLayer()`
 */
TACTILE_IO_API_QUERY
auto GetTileLayer(Layer& layer) -> TileLayer&;

/// \copydoc GetTileLayer()
TACTILE_IO_API_QUERY
auto GetTileLayer(const Layer& layer) -> const TileLayer&;

/**
 * \brief Returns the object layer data associated with a layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return the object layer data.
 *
 * \throws TactileError if the layer isn't an object layer.
 *
 * \see `MarkAsObjectLayer()`
 */
TACTILE_IO_API_QUERY
auto GetObjectLayer(Layer& layer) -> ObjectLayer&;

/// \copydoc GetObjectLayer()
TACTILE_IO_API_QUERY
auto GetObjectLayer(const Layer& layer) -> const ObjectLayer&;

/**
 * \brief Returns the group layer data associated with a layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return the group layer data.
 *
 * \throws TactileError if the layer isn't a group layer.
 *
 * \see `MarkAsGroupLayer()`
 */
TACTILE_IO_API_QUERY
auto GetGroupLayer(Layer& layer) -> GroupLayer&;

/// \copydoc GetGroupLayer()
TACTILE_IO_API_QUERY
auto GetGroupLayer(const Layer& layer) -> const GroupLayer&;

/**
 * \brief Sets the ID associated with a layer.
 *
 * \param layer the layer that will be modified.
 *
 * \param id the new ID of the layer.
 */
TACTILE_IO_API
void SetId(Layer& layer, int32 id);

/**
 * \brief Sets the local (i.e. relative to layer siblings) index of a layer.
 *
 * \param layer the layer that will be modified.
 * \param index the layer index.
 */
TACTILE_IO_API
void SetIndex(Layer& layer, usize index);

/**
 * \brief Sets the name associated with a layer.
 *
 * \pre The supplied string cannot be null.
 *
 * \param layer the layer that will be modified.
 * \param name the name of the layer.
 */
TACTILE_IO_API
void SetName(Layer& layer, CStr name);

/**
 * \brief Sets the opacity of a layer.
 *
 * \details The supplied opacity is clamped to [0, 1].
 *
 * \param layer the layer that will be modified.
 * \param opacity the opacity of the layer, in the range [0, 1].
 */
TACTILE_IO_API
void SetOpacity(Layer& layer, float opacity);

/**
 * \brief Sets the visibility of a layer.
 *
 * \param layer the layer that will be modified.
 * \param visible `true` if the layer should be visible; `false` otherwise.
 */
TACTILE_IO_API
void SetVisible(Layer& layer, bool visible);

/**
 * \brief Returns the ID associated with a layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return the layer ID.
 */
TACTILE_IO_API_QUERY
auto GetId(const Layer& layer) -> int32;

/**
 * \brief Returns the local index associated with a layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return the layer index.
 */
TACTILE_IO_API_QUERY
auto GetIndex(const Layer& layer) -> usize;

/**
 * \brief Returns the name of a layer.
 *
 * \details The default layer name is the empty string.
 *
 * \param layer the layer that will be queried.
 *
 * \return the layer name.
 */
TACTILE_IO_API_QUERY
auto GetName(const Layer& layer) -> CStr;

/**
 * \brief Returns the opacity of a layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return the opacity of the layer, in the range [0, 1].
 */
TACTILE_IO_API_QUERY
auto GetOpacity(const Layer& layer) -> float;

/**
 * \brief Indicates whether or not a layer is visible.
 *
 * \param layer the layer that will be queried.
 *
 * \return `true` if the layer is visible; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsVisible(const Layer& layer) -> bool;

/**
 * \brief Returns the specific type of a layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return the layer type.
 */
TACTILE_IO_API_QUERY
auto GetType(const Layer& layer) -> LayerType;

/**
 * \brief Indicates whether or not a layer is a tile layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return `true` if the layer is a tile layer; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsTileLayer(const Layer& layer) -> bool;

/**
 * \brief Indicates whether or not a layer is an object layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return `true` if the layer is an object layer; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsObjectLayer(const Layer& layer) -> bool;

/**
 * \brief Indicates whether or not a layer is a group layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return `true` if the layer is a group layer; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsGroupLayer(const Layer& layer) -> bool;

/**
 * \brief Reserves enough space for the specified amount of properties in a layer.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddProperty()`.
 *
 * \param layer the layer that will be affected.
 * \param n the number of properties to allocate enough space for.
 *
 * \see `AddProperty(Layer&)`
 */
TACTILE_IO_API
void ReserveProperties(Layer& layer, usize n);

/**
 * \brief Adds a property to a layer.
 *
 * \param layer the layer that a property will be added to.
 *
 * \return a reference to the created property.
 *
 * \see `ReserveProperties(Layer&)`
 */
TACTILE_IO_API_QUERY
auto AddProperty(Layer& layer) -> Property&;

/**
 * \brief Returns the amount of properties associated with a layer.
 *
 * \param layer the layer that will be queried.
 *
 * \return the amount of properties.
 */
TACTILE_IO_API_QUERY
auto GetPropertyCount(const Layer& layer) -> usize;

/**
 * \brief Returns the property at the specified index in a layer.
 *
 * \details This function will throw if there is no property associated with the index.
 *
 * \param layer the layer that will be queried.
 * \param index the index of the desired property.
 *
 * \return a reference to the property at the specified index.
 */
TACTILE_IO_API_QUERY
auto GetProperty(Layer& layer, usize index) -> Property&;

/// \copydoc GetProperty()
TACTILE_IO_API_QUERY
auto GetProperty(const Layer& layer, usize index) -> const Property&;

TACTILE_IO_API
void ReserveTiles(TileLayer& layer, usize nRows, usize nCols);

TACTILE_IO_API
void SetTile(TileLayer& layer, usize row, usize column, int32 tile);

TACTILE_IO_API_QUERY
auto GetRowCount(const TileLayer& layer) -> usize;

TACTILE_IO_API_QUERY
auto GetColumnCount(const TileLayer& layer) -> usize;

TACTILE_IO_API_QUERY
auto GetTile(const TileLayer& layer, usize row, usize column) -> int32;

/**
 * \brief Reserves enough space for the specified amount of objects in an object layer.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddObject()`.
 *
 * \param map the map that will be affected.
 * \param n the number of objects to allocate enough space for.
 *
 * \see `AddObject(ObjectLayer&)`
 */
TACTILE_IO_API
void ReserveObjects(ObjectLayer& layer, usize n);

/**
 * \brief Adds a new object to an object layer.
 *
 * \param layer the object layer that will have an object added to it.
 *
 * \return the created object.
 *
 * \see `ReserveObjects(ObjectLayer&, usize)`
 */
TACTILE_IO_API_QUERY
auto AddObject(ObjectLayer& layer) -> Object&;

/**
 * \brief Returns the amount of objects associated with an object layer.
 *
 * \param layer the object layer that will be queried.
 *
 * \return the amount of objects.
 */
TACTILE_IO_API_QUERY
auto GetObjectCount(const ObjectLayer& layer) -> usize;

/**
 * \brief Returns the object at the specified index in an object layer.
 *
 * \param layer the object layer that will be queried.
 * \param index the index of the desired object.
 *
 * \return the object at the specified index.
 *
 * \see `GetObjectCount(const ObjectLayer&)`
 */
TACTILE_IO_API_QUERY
auto GetObject(const ObjectLayer& layer, usize index) -> const Object&;

/**
 * \brief Reserves enough space for the specified amount of layers in a group layer.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddLayer()`.
 *
 * \param map the map that will be affected.
 * \param n the number of layers to allocate enough space for.
 *
 * \see `AddLayer(GroupLayer&)`
 */
TACTILE_IO_API
void ReserveLayers(GroupLayer& layer, usize n);

/**
 * \brief Adds a new layer as a child layer of a group layer.
 *
 * \param layer the group layer that will have a layer added to it.
 *
 * \return the created layer.
 *
 * \see `ReserveLayers(GroupLayer&, usize)`
 */
TACTILE_IO_API_QUERY
auto AddLayer(GroupLayer& layer) -> Layer&;

/**
 * \brief Returns the amount of child layers associated with a group layer.
 *
 * \param layer the group layer that will be queried.
 *
 * \return the amount of child layers.
 */
TACTILE_IO_API_QUERY
auto GetLayerCount(const GroupLayer& layer) -> usize;

/**
 * \brief Returns the child layer at the specified index in a group layer.
 *
 * \param layer the group layer that will be queried.
 * \param index the index of the desired child layer.
 *
 * \return the child layer at the specified index.
 *
 * \see `GetLayerCount(const GroupLayer&)`
 */
TACTILE_IO_API_QUERY
auto GetLayer(const GroupLayer& layer, usize index) -> const Layer&;

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_LAYER_HPP_

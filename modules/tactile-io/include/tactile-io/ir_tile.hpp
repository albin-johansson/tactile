#ifndef TACTILE_IO_IR_TILE_HPP_
#define TACTILE_IO_IR_TILE_HPP_

#include "api.hpp"
#include "ir_common.hpp"

namespace tactile::IO {

/// \name Tile API
/// \{

/**
 * \brief Sets the *local* tile ID associated with a tile info object.
 *
 * \param tile the tile info object that will be affected.
 * \param id the local ID of the associated tile.
 */
TACTILE_IO_API
void SetId(Tile& tile, int32 id);

/**
 * \brief Reserves enough space for the specified amount of animation frames for a tile.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddAnimationFrame()`.
 *
 * \param tile the tile info object that will be affected.
 * \param n the number of frames to allocate enough space for.
 *
 * \see `AddAnimationFrame()`
 */
TACTILE_IO_API
void ReserveAnimationFrames(Tile& tile, usize n);

/**
 * \brief Reserves enough space for the specified amount of associated objects for a tile.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddObject()`.
 *
 * \param tile the tile info object that will be affected.
 * \param n the number of objects to allocate enough space for.
 *
 * \see `AddObject(Tile&)`
 */
TACTILE_IO_API
void ReserveObjects(Tile& tile, usize n);

/**
 * \brief Reserves enough space for the specified amount of properties for a tile.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddProperty()`.
 *
 * \param tile the tile info object that will be affected.
 * \param n the number of properties to allocate enough space for.
 *
 * \see `AddProperty(Tile&)`
 */
TACTILE_IO_API
void ReserveProperties(Tile& tile, usize n);

/**
 * \brief Adds an animation frame to a tile.
 *
 * \param tile the tile info object that an animation frame will be added to.
 *
 * \return a reference to the created animation frame.
 *
 * \see `ReserveAnimationFrames()`
 */
TACTILE_IO_API_QUERY
auto AddAnimationFrame(Tile& tile) -> AnimationFrame&;

/**
 * \brief Adds an object to a tile.
 *
 * \param tile the tile info object that an object will be added to.
 *
 * \return a reference to the created object.
 *
 * \see `ReserveObjects(Tile&)`
 */
TACTILE_IO_API_QUERY
auto AddObject(Tile& tile) -> Object&;

/**
 * \brief Adds a property to a tile.
 *
 * \param tile the tile info object that a property will be added to.
 *
 * \return a reference to the created property.
 *
 * \see `ReserveProperties(Tile&)`
 */
TACTILE_IO_API_QUERY
auto AddProperty(Tile& tile) -> Property&;

/**
 * \brief Indicates whether or not a tile provides any meta-information that is worth
 * saving.
 *
 * \details This function is intended to be used to avoid emitting tile nodes in tileset
 * files that provide no useful information.
 *
 * \param tile the tile info object that will be queried.
 *
 * \return `true` if the tile has information worth saving; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsWorthSaving(const Tile& tile) -> bool;

/**
 * \brief Returns the local tile ID associated with a tile info object.
 *
 * \param tile the tile info object that will be queried.
 *
 * \return a local tile ID.
 */
TACTILE_IO_API_QUERY
auto GetId(const Tile& tile) -> int32;

/**
 * \brief Returns the total number of animation frames associated with a tile.
 *
 * \param tile the tile info object that will be queried.
 *
 * \return the amount of animation frames.
 */
TACTILE_IO_API_QUERY
auto GetAnimationFrameCount(const Tile& tile) -> usize;

/**
 * \brief Returns the total number of objects associated with a tile.
 *
 * \param tile the tile info object that will be queried.
 *
 * \return the amount of associated objects.
 */
TACTILE_IO_API_QUERY
auto GetObjectCount(const Tile& tile) -> usize;

/**
 * \brief Returns the total number of properties associated with a tile.
 *
 * \param tile the tile info object that will be queried.
 *
 * \return the amount of associated properties.
 */
TACTILE_IO_API_QUERY
auto GetPropertyCount(const Tile& tile) -> usize;

/**
 * \brief Returns the animation frame at the specified index in a tile.
 *
 * \details This function will throw if there is no frame associated with the index.
 *
 * \param tile the tile info object that will be queried.
 * \param index the index of the desired animation frame.
 *
 * \return a reference to the frame at the specified index.
 */
TACTILE_IO_API_QUERY
auto GetAnimationFrame(Tile& tile, usize index) -> AnimationFrame&;

/// \copydoc GetAnimationFrame()
TACTILE_IO_API_QUERY
auto GetAnimationFrame(const Tile& tile, usize index) -> const AnimationFrame&;

/**
 * \brief Returns the object at the specified index in a tile.
 *
 * \details This function will throw if there is no object associated with the index.
 *
 * \param tile the tile info object that will be queried.
 * \param index the index of the desired object.
 *
 * \return a reference to the object at the specified index.
 */
TACTILE_IO_API_QUERY
auto GetObject(Tile& tile, usize index) -> Object&;

/// \copydoc GetObject()
TACTILE_IO_API_QUERY
auto GetObject(const Tile& tile, usize index) -> const Object&;

/**
 * \brief Returns the property at the specified index in a tile.
 *
 * \details This function will throw if there is no property associated with the index.
 *
 * \param tile the tile info object that will be queried.
 * \param index the index of the desired property.
 *
 * \return a reference to the property at the specified index.
 */
TACTILE_IO_API_QUERY
auto GetProperty(Tile& tile, usize index) -> Property&;

/// \copydoc GetProperty()
TACTILE_IO_API_QUERY
auto GetProperty(const Tile& tile, usize index) -> const Property&;

/// \} End of tile API

}  // namespace tactile::IO

#endif  // TACTILE_IO_IR_TILE_HPP_

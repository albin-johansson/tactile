#ifndef TACTILE_IO_IR_MAP_HPP_
#define TACTILE_IO_IR_MAP_HPP_

#include <memory>  // unique_ptr

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/// \name Map API
/// \{

/**
 * \brief Creates a new `Map` instance, which must be freed using `DeleteMap()`.
 *
 * \note This function is not intended to be called explicitly in client code, use
 * `CreateMap()` instead in order to avoid having to manually manage memory.
 *
 * \return a pointer to the created `Map`, it is your responsibility to free the
 * associated memory if you call this function.
 *
 * \see `DeleteMap()`
 * \see `CreateMap()`
 */
TACTILE_IO_API_QUERY
auto NewMap() -> Map*;

/**
 * \brief Destroys a `Map` instance allocated with `NewMap()`.
 *
 * \note This function is not intended to be called explicitly in client code, use
 * `CreateMap()` instead in order to avoid having to manually manage memory.
 *
 * \param map a pointer to a `Map` instance created with `NewMap()`, can safely be null.
 *
 * \see `NewMap()`
 * \see `CreateMap()`
 */
TACTILE_IO_API
void DeleteMap(Map* map) noexcept;

/**
 * \brief A simple custom `Map` deleter for use with `std::unique_ptr`.
 *
 * \details Just like `NewMap()` and `DeleteMap()`, you should not have to bother with
 * this type explicitly. It is used by the unique pointer returned by `CreateMap()`.
 *
 * \see `MapPtr`
 */
struct TACTILE_IO_API MapDeleter final {
  void operator()(Map* map) noexcept
  {
    DeleteMap(map);
  }
};

/// \brief Alias for a unique pointer to a `Map` instance.

/**
 * \brief Alias for a unique pointer to a `Map` instance, using of a custom deleter.
 *
 * \details This should be considered the only safe smart pointer for use with `Map`
 * instances.
 *
 * \see `CreateMap()`
 * \see `NewMap()`
 * \see `DeleteMap()`
 */
using MapPtr = std::unique_ptr<Map, MapDeleter>;

/**
 * \brief Creates and returns a unique pointer to an empty `Map` instance.
 *
 * \details This function is the recommended way to construct `Map` instances, since you
 * don't have to worry about managing the allocated memory (which is not the case with
 * `NewMap()`).
 *
 * \details The `Map` type is the only type in the library for which you have to think
 * about using the "right" factory function, the lifetimes of all other types are managed
 * internally by `Map` instances.
 *
 * \note This function cannot simply be replaced by a call to `std::make_unique()`, since
 * you need to use a custom deleter for the incomplete `Map` type.
 *
 * \return a unique pointer to a `Map`.
 */
inline auto CreateMap() -> MapPtr
{
  return MapPtr{NewMap()};
}

/**
 * \brief Sets the file path associated with a map.
 *
 * \pre The supplied string cannot be null.
 *
 * \details This function will convert the supplied path to an absolute path.
 *
 * \param map the map that will be affected.
 * \param path the file path of the map, i.e. where the map data is stored.
 */
TACTILE_IO_API
void SetPath(Map& map, CPathStr path);

/**
 * \brief Sets the next available layer ID in a map.
 *
 * \param map the map that will be affected.
 * \param id the next available layer ID.
 */
TACTILE_IO_API
void SetNextLayerId(Map& map, int32 id);

/**
 * \brief Sets the next available object ID in a map.
 *
 * \param map the map that will be affected.
 * \param id the next available object ID.
 */
TACTILE_IO_API
void SetNextObjectId(Map& map, int32 id);

/**
 * \brief Sets the width of the tiles in a map.
 *
 * \pre The supplied tile width must be greater than zero.
 *
 * \param map the map that will be affected.
 * \param width the map tile width.
 */
TACTILE_IO_API
void SetTileWidth(Map& map, int32 width);

/**
 * \brief Sets the height of the tiles in a map.
 *
 * \pre The supplied tile height must be greater than zero.
 *
 * \param map the map that will be affected.
 * \param height the map tile height.
 */
TACTILE_IO_API
void SetTileHeight(Map& map, int32 height);

/**
 * \brief Sets the amount of tile rows in a map.
 *
 * \param map the map that will be affected.
 * \param rows the number of tile rows.
 */
TACTILE_IO_API
void SetRowCount(Map& map, usize rows);

/**
 * \brief Sets the amount of tile columns in a map.
 *
 * \param map the map that will be affected.
 * \param columns the number of tile rows.
 */
TACTILE_IO_API
void SetColumnCount(Map& map, usize columns);

/**
 * \brief Reserves enough space for the specified amount of tilesets in a map.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddTileset()`.
 *
 * \param map the map that will be affected.
 * \param n the number of tilesets to allocate enough space for.
 *
 * \see `AddTileset()`
 */
TACTILE_IO_API
void ReserveTilesets(Map& map, usize n);

/**
 * \brief Reserves enough space for the specified amount of layers in a map.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddLayer()`.
 *
 * \param map the map that will be affected.
 * \param n the number of layers to allocate enough space for.
 *
 * \see `AddLayer(Map&)`
 */
TACTILE_IO_API
void ReserveLayers(Map& map, usize n);

/**
 * \brief Reserves enough space for the specified amount of properties in a map.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddProperty()`.
 *
 * \param map the map that will be affected.
 * \param n the number of properties to allocate enough space for.
 *
 * \see `AddProperty(Map&)`
 */
TACTILE_IO_API
void ReserveProperties(Map& map, usize n);

/**
 * \brief Adds a tileset to a map.
 *
 * \param map the map that a tileset will be added to.
 *
 * \return a reference to the created tileset.
 *
 * \see `ReserveTilesets()`
 */
TACTILE_IO_API_QUERY
auto AddTileset(Map& map) -> Tileset&;

/**
 * \brief Adds a layer to a map.
 *
 * \param map the map that a layer will be added to.
 *
 * \return a reference to the created layer.
 *
 * \see `ReserveLayers()`
 */
TACTILE_IO_API_QUERY
auto AddLayer(Map& map) -> Layer&;

/**
 * \brief Adds a property to a map.
 *
 * \param map the map that a property will be added to.
 *
 * \return a reference to the created property.
 *
 * \see `ReserveProperties(Map&)`
 */
TACTILE_IO_API_QUERY
auto AddProperty(Map& map) -> Property&;

/**
 * \brief Returns the absolute file path associated with a map.
 *
 * \details You must copy the returned string if you want to store it for longer than the
 * lifetime of the `Map` instance.
 *
 * \param map the map that will be queried.
 *
 * \return an absolute file path.
 */
TACTILE_IO_API_QUERY
auto GetPath(const Map& map) -> CPathStr;

/**
 * \brief Returns the next available layer ID in a map.
 *
 * \param map the map that will be queried.
 *
 * \return the next available layer ID.
 */
TACTILE_IO_API_QUERY
auto GetNextLayerId(const Map& map) -> int32;

/**
 * \brief Returns the next available object ID in a map.
 *
 * \param map the map that will be queried.
 *
 * \return the next available object ID.
 */
TACTILE_IO_API_QUERY
auto GetNextObjectId(const Map& map) -> int32;

/**
 * \brief Returns the tile width in a map.
 *
 * \param map the map that will be queried.
 *
 * \return the map tile width.
 */
TACTILE_IO_API_QUERY
auto GetTileWidth(const Map& map) -> int32;

/**
 * \brief Returns the tile height in a map.
 *
 * \param map the map that will be queried.
 *
 * \return the map tile height.
 */
TACTILE_IO_API_QUERY
auto GetTileHeight(const Map& map) -> int32;

/**
 * \brief Returns the amount of tile rows in a map.
 *
 * \param map the map that will be queried.
 *
 * \return the amount of rows.
 */
TACTILE_IO_API_QUERY
auto GetRowCount(const Map& map) -> usize;

/**
 * \brief Returns the amount of tile columns in a map.
 *
 * \param map the map that will be queried.
 *
 * \return the amount of columns.
 */
TACTILE_IO_API_QUERY
auto GetColumnCount(const Map& map) -> usize;

/**
 * \brief Returns the amount of tilesets associated with a map.
 *
 * \details This function can be used to safely iterate all of the tilesets associated
 * with a map, like in the following example.
 * \code{cpp}
 * const auto count = GetTilesetCount(map);
 * for (usize i = 0; i < count; ++i) {
 *   const auto& tileset = GetTileset(map, i);
 *   // ...
 * }
 * \endcode
 *
 * \param map the map that will be queried.
 *
 * \return the amount of tilesets.
 */
TACTILE_IO_API_QUERY
auto GetTilesetCount(const Map& map) -> usize;

/**
 * \brief Returns the amount of layers associated with a map.
 *
 * \details This function can be used to safely iterate all of the layers associated
 * with a map, like in the following example.
 * \code{cpp}
 * const auto count = GetLayerCount(map);
 * for (usize i = 0; i < count; ++i) {
 *   const auto& layer = GetLayer(map, i);
 *   // ...
 * }
 * \endcode
 *
 * \param map the map that will be queried.
 *
 * \return the amount of tilesets.
 */
TACTILE_IO_API_QUERY
auto GetLayerCount(const Map& map) -> usize;

/**
 * \brief Returns the amount of properties associated with a map.
 *
 * \param map the map that will be queried.
 *
 * \return the amount of properties.
 */
TACTILE_IO_API_QUERY
auto GetPropertyCount(const Map& map) -> usize;

/**
 * \brief Returns the tileset at the specified index in a map.
 *
 * \details This function will throw if there is no tileset associated with the index.
 *
 * \param map the map that will be queried.
 * \param index the index of the desired tileset.
 *
 * \return a reference to the tileset at the specified index.
 */
TACTILE_IO_API_QUERY
auto GetTileset(Map& map, usize index) -> Tileset&;

/// \copydoc GetTileset()
TACTILE_IO_API_QUERY
auto GetTileset(const Map& map, usize index) -> const Tileset&;

/**
 * \brief Returns the layer at the specified index in a map.
 *
 * \details This function will throw if there is no layer associated with the index.
 *
 * \param map the map that will be queried.
 * \param index the index of the desired layer.
 *
 * \return a reference to the layer at the specified index.
 */
TACTILE_IO_API_QUERY
auto GetLayer(Map& map, usize index) -> Layer&;

/// \copydoc GetLayer()
TACTILE_IO_API_QUERY
auto GetLayer(const Map& map, usize index) -> const Layer&;

/**
 * \brief Returns the property at the specified index in a map.
 *
 * \details This function will throw if there is no property associated with the index.
 *
 * \param map the map that will be queried.
 * \param index the index of the desired property.
 *
 * \return a reference to the property at the specified index.
 */
TACTILE_IO_API_QUERY
auto GetProperty(Map& map, usize index) -> Property&;

/// \copydoc GetProperty()
TACTILE_IO_API_QUERY
auto GetProperty(const Map& map, usize index) -> const Property&;

/// \} End of map API

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_MAP_HPP_

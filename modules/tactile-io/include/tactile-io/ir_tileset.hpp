#ifndef TACTILE_IO_IR_TILESET_HPP_
#define TACTILE_IO_IR_TILESET_HPP_

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/// \name Tileset API
/// \{

/**
 * \brief Sets the name of a tileset.
 *
 * \pre The supplied string cannot be null.
 *
 * \param tileset the tileset that will be affected.
 * \param name the name of the tileset.
 */
TACTILE_IO_API void SetName(Tileset& tileset, CStr name);

/**
 * \brief Sets the first global tile ID associated with a tileset.
 *
 * \details The specified first global ID is effectively an offset to the tile identifiers
 * used by the tileset. This is needed in order to make it possible to use multiple
 * tilesets.
 *
 * \param tileset the tileset that will be affected.
 * \param id the first associated global tile ID.
 */
TACTILE_IO_API void SetFirstGlobalId(Tileset& tileset, int32 id);

/**
 * \brief Sets the width of tiles in a tileset.
 *
 * \note Do not confuse the size of tiles in a tileset and the size of tiles in maps.
 *
 * \param tileset the tileset that will be affected.
 * \param width the width of the tileset tiles.
 */
TACTILE_IO_API void SetTileWidth(Tileset& tileset, int32 width);

/**
 * \brief Sets the height of tiles in a tileset.
 *
 * \note Do not confuse the size of tiles in a tileset and the size of tiles in maps.
 *
 * \param tileset the tileset that will be affected.
 * \param height the height of the tileset tiles.
 */
TACTILE_IO_API void SetTileHeight(Tileset& tileset, int32 height);

/**
 * \brief Sets the total number of tiles in a tileset.
 *
 * \param tileset the tileset that will be affected.
 * \param count the amount of tiles.
 */
TACTILE_IO_API void SetTileCount(Tileset& tileset, int32 count);

/**
 * \brief Sets the amount of columns in a tileset.
 *
 * \param tileset the tileset that will be affected.
 * \param columns the amount of tile columns.
 */
TACTILE_IO_API void SetColumnCount(Tileset& tileset, int32 columns);

/**
 * \brief Sets the associated image path of a tileset.
 *
 * \pre The supplied string cannot be null.
 *
 * \details This function will convert the supplied path to an absolute path.
 *
 * \param tileset the tileset that will be affected.
 * \param path the file path to the associated image.
 */
TACTILE_IO_API void SetImagePath(Tileset& tileset, CPathStr path);

/**
 * \brief Sets the associated image width of a tileset.
 *
 * \pre The supplied width must be greater than zero.
 *
 * \param tileset the tileset that will be affected.
 * \param width the width of the image.
 */
TACTILE_IO_API void SetImageWidth(Tileset& tileset, int32 width);

/**
 * \brief Sets the associated image height of a tileset.
 *
 * \pre The supplied height must be greater than zero.
 *
 * \param tileset the tileset that will be affected.
 * \param height the height of the image.
 */
TACTILE_IO_API void SetImageHeight(Tileset& tileset, int32 height);

/**
 * \brief Reserves enough space for the specified amount of tile info objects in a
 * tileset.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddTile()`.
 *
 * \param tileset the tileset that will be affected.
 * \param n the number of tile info objects to allocate enough space for.
 *
 * \see `AddTile()`
 */
TACTILE_IO_API void ReserveTiles(Tileset& tileset, usize n);

/**
 * \brief Reserves enough space for the specified amount of properties in a tileset.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddProperty(Tileset&)`.
 *
 * \param tileset the tileset that will be affected.
 * \param n the number of properties to allocate enough space for.
 *
 * \see `AddProperty(Tileset&)`
 */
TACTILE_IO_API void ReserveProperties(Tileset& tileset, usize n);

/**
 * \brief Adds a tile info object to a tileset.
 *
 * \param tileset the tileset that a tile info object will be added to.
 *
 * \return a reference to the created tile info object.
 *
 * \see `ReserveTiles()`
 */
TACTILE_IO_API_QUERY auto AddTile(Tileset& tileset) -> Tile&;

/**
 * \brief Adds a property to a tileset.
 *
 * \param tileset the tileset that a property will be added to.
 *
 * \return a reference to the created property.
 *
 * \see `ReserveTiles(Tileset&)`
 */
TACTILE_IO_API_QUERY auto AddProperty(Tileset& tileset) -> Property&;

/**
 * \brief Returns the name of a tileset.
 *
 * \details You must copy the returned string if you want to store it for longer than the
 * lifetime of the `Tileset` instance.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the tileset name.
 */
TACTILE_IO_API_QUERY auto GetName(const Tileset& tileset) -> CStr;

/**
 * \brief Returns the first global tile ID associated with a tileset.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the first global tile ID.
 */
TACTILE_IO_API_QUERY auto GetFirstGlobalId(const Tileset& tileset) -> int32;

/**
 * \brief Returns the width of tiles in a tileset.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the tileset tile width.
 */
TACTILE_IO_API_QUERY auto GetTileWidth(const Tileset& tileset) -> int32;

/**
 * \brief Returns the height of tiles in a tileset.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the tileset tile height.
 */
TACTILE_IO_API_QUERY auto GetTileHeight(const Tileset& tileset) -> int32;

/**
 * \brief Returns the total amount of tiles in a tileset.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the amount of tiles in the tileset.
 */
TACTILE_IO_API_QUERY auto GetTileCount(const Tileset& tileset) -> int32;

/**
 * \brief Returns the amount of tile columns in a tileset.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the amount of tile columns in the tileset.
 */
TACTILE_IO_API_QUERY auto GetColumnCount(const Tileset& tileset) -> int32;

/**
 * \brief Returns the path to the associated image of a tileset.
 *
 * \details You must copy the returned string if you want to store it for longer than the
 * lifetime of the `Tileset` instance.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return an absolute path to the associated image.
 */
TACTILE_IO_API_QUERY auto GetImagePath(const Tileset& tileset) -> CPathStr;

/**
 * \brief Returns the width of the image associated with a tileset.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the tileset image width.
 */
TACTILE_IO_API_QUERY auto GetImageWidth(const Tileset& tileset) -> int32;

/**
 * \brief Returns the height of the image associated with a tileset.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the tileset image height.
 */
TACTILE_IO_API_QUERY auto GetImageHeight(const Tileset& tileset) -> int32;

/**
 * \brief Returns the total amount of tile info objects in a tileset.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the amount of tile info objects.
 *
 * \see `GetTileInfo()`
 */
TACTILE_IO_API_QUERY auto GetTileInfoCount(const Tileset& tileset) -> usize;

/**
 * \brief Returns the tile info object at the specified index in a tileset.
 *
 * \details This function will throw if there is no tile info object associated with the
 * index.
 *
 * \param tileset the tileset that will be queried.
 * \param index the index of the tile info object.
 *
 * \return a reference to the tile info object at the specified index.
 */
TACTILE_IO_API_QUERY auto GetTileInfo(Tileset& tileset, usize index) -> Tile&;

/// \copydoc GetTileInfo()
TACTILE_IO_API_QUERY auto GetTileInfo(const Tileset& tileset, usize index) -> const Tile&;

/**
 * \brief Returns the total amount of properties in a tileset.
 *
 * \param tileset the tileset that will be queried.
 *
 * \return the amount of properties.
 *
 * \see `GetProperty()`
 */
TACTILE_IO_API_QUERY auto GetPropertyCount(const Tileset& tileset) -> usize;

/**
 * \brief Returns the property at the specified index in a tileset.
 *
 * \details This function will throw if there is no property associated with the index.
 *
 * \param tileset the tileset that will be queried.
 * \param index the index of the desired property.
 *
 * \return a reference to the property at the specified index.
 */
TACTILE_IO_API_QUERY auto GetProperty(Tileset& tileset, usize index) -> Property&;

/// \copydoc GetPropertyCount()
TACTILE_IO_API_QUERY auto GetProperty(const Tileset& tileset, usize index)
    -> const Property&;

/// \} End of tileset API

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_TILESET_HPP_

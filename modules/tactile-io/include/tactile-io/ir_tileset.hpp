#ifndef TACTILE_IO_IR_TILESET_HPP_
#define TACTILE_IO_IR_TILESET_HPP_

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/// \name Tileset API
/// \{

TACTILE_IO_API void SetName(Tileset& tileset, CStr name);
TACTILE_IO_API void SetFirstGlobalId(Tileset& tileset, int32 id);
TACTILE_IO_API void SetTileWidth(Tileset& tileset, int32 width);
TACTILE_IO_API void SetTileHeight(Tileset& tileset, int32 height);
TACTILE_IO_API void SetTileCount(Tileset& tileset, int32 count);
TACTILE_IO_API void SetColumnCount(Tileset& tileset, int32 columns);
TACTILE_IO_API void SetImagePath(Tileset& tileset, CPathStr path);
TACTILE_IO_API void SetImageWidth(Tileset& tileset, int32 width);
TACTILE_IO_API void SetImageHeight(Tileset& tileset, int32 height);

TACTILE_IO_API void ReserveTiles(Tileset& tileset, usize n);
TACTILE_IO_API void ReserveProperties(Tileset& tileset, usize n);

TACTILE_IO_API_QUERY auto AddTile(Tileset& tileset) -> Tile&;
TACTILE_IO_API_QUERY auto AddProperty(Tileset& tileset) -> Property&;

TACTILE_IO_API_QUERY auto GetName(const Tileset& tileset) -> CStr;
TACTILE_IO_API_QUERY auto GetFirstGlobalId(const Tileset& tileset) -> int32;
TACTILE_IO_API_QUERY auto GetTileWidth(const Tileset& tileset) -> int32;
TACTILE_IO_API_QUERY auto GetTileHeight(const Tileset& tileset) -> int32;
TACTILE_IO_API_QUERY auto GetTileCount(const Tileset& tileset) -> int32;
TACTILE_IO_API_QUERY auto GetColumnCount(const Tileset& tileset) -> int32;
TACTILE_IO_API_QUERY auto GetImagePath(const Tileset& tileset) -> CPathStr;
TACTILE_IO_API_QUERY auto GetImageWidth(const Tileset& tileset) -> int32;
TACTILE_IO_API_QUERY auto GetImageHeight(const Tileset& tileset) -> int32;

TACTILE_IO_API_QUERY auto GetTileInfoCount(const Tileset& tileset) -> usize;
TACTILE_IO_API_QUERY auto GetTileInfo(const Tileset& tileset, usize index) -> const Tile&;

TACTILE_IO_API_QUERY auto GetPropertyCount(const Tileset& tileset) -> usize;
TACTILE_IO_API_QUERY auto GetProperty(const Tileset& tileset, usize index)
    -> const Property&;

/// \} End of tileset API

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_TILESET_HPP_

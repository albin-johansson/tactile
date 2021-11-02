#ifndef TACTILE_IO_IR_TILE_HPP_
#define TACTILE_IO_IR_TILE_HPP_

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/// \name Tile API
/// \{

TACTILE_IO_API void SetId(Tile& tile, int32 id);

TACTILE_IO_API void ReserveAnimationFrames(Tile& tile, usize n);
TACTILE_IO_API void ReserveObjects(Tile& tile, usize n);
TACTILE_IO_API void ReserveProperties(Tile& tile, usize n);

TACTILE_IO_API_QUERY auto AddAnimationFrame(Tile& tile) -> AnimationFrame&;
TACTILE_IO_API_QUERY auto AddObject(Tile& tile) -> Object&;
TACTILE_IO_API_QUERY auto AddProperty(Tile& tile) -> Property&;

TACTILE_IO_API_QUERY auto IsWorthSaving(const Tile& tile) -> bool;
TACTILE_IO_API_QUERY auto GetId(const Tile& tile) -> int32;

TACTILE_IO_API_QUERY auto GetAnimationFrameCount(const Tile& tile) -> usize;
TACTILE_IO_API_QUERY auto GetObjectCount(const Tile& tile) -> usize;
TACTILE_IO_API_QUERY auto GetPropertyCount(const Tile& tile) -> usize;

TACTILE_IO_API_QUERY auto GetAnimationFrame(const Tile& tile, usize index)
    -> const AnimationFrame&;
TACTILE_IO_API_QUERY auto GetObject(const Tile& tile, usize index) -> const Object&;
TACTILE_IO_API_QUERY auto GetProperty(const Tile& tile, usize index) -> const Property&;

/// \} End of tile API

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_TILE_HPP_

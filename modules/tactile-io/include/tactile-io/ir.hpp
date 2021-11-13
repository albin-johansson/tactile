#pragma once

#include <concepts>  // invocable
#include <memory>    // unique_ptr

#include <tactile_def.hpp>

#include "api.hpp"
#include "ir_common.hpp"
#include "ir_layer.hpp"
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

TACTILE_IO_API
void SetTile(AnimationFrame& frame, int32 tile);

TACTILE_IO_API
void SetDuration(AnimationFrame& frame, int32 ms);

TACTILE_IO_API_QUERY
auto GetTile(const AnimationFrame& frame) -> int32;

TACTILE_IO_API_QUERY
auto GetDuration(const AnimationFrame& frame) -> int32;

/// \} End of animation API

/// \} End of group io

}  // namespace Tactile::IO

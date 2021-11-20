#ifndef TACTILE_IO_IR_ANIMATION_FRAME_HPP_
#define TACTILE_IO_IR_ANIMATION_FRAME_HPP_

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

/**
 * \brief Sets the tile associated with an animation frame.
 *
 * \param frame the frame that will be modified.
 * \param tile the _local_ ID of the tile to show during the frame.
 */
TACTILE_IO_API
void SetTile(AnimationFrame& frame, int32 tile);

/**
 * \brief Sets the duration of an animation frame.
 *
 * \param frame the frame that will be modified.
 * \param ms the duration of the frame, in milliseconds.
 */
TACTILE_IO_API
void SetDuration(AnimationFrame& frame, int32 ms);

/**
 * \brief Returns the identifier of the tile shown during an animation frame.
 *
 * \param frame the animation frame that will be queried.
 *
 * \return the local ID of the associated tile.
 */
TACTILE_IO_API_QUERY
auto GetTile(const AnimationFrame& frame) -> int32;

/**
 * \brief Returns the duration of an animation frame.
 *
 * \param frame the animation frame that will be queried.
 *
 * \return the duration of the frame, in milliseconds.
 */
TACTILE_IO_API_QUERY
auto GetDuration(const AnimationFrame& frame) -> int32;

/// \} End of animation API

/// \} End of group io

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_ANIMATION_FRAME_HPP_

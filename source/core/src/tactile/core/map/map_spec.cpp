// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/map_spec.hpp"

namespace tactile {

auto is_valid(const MapSpec& spec) -> bool
{
  return (spec.extent.rows > 0) &&    //
         (spec.extent.cols > 0) &&    //
         (spec.tile_size.x() > 0) &&  //
         (spec.tile_size.y() > 0);
}

}  // namespace tactile

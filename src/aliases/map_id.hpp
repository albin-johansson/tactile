#pragma once

#include <nenya.hpp>  // strong_type

namespace Tactile {

namespace Tags {
struct MapIdTag;
}  // namespace Tags

using map_id = nenya::strong_type<int, Tags::MapIdTag>;

}  // namespace Tactile

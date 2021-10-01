#pragma once

#include <nenya.hpp>  // strong_type

namespace Tactile {

namespace Tags {
struct MapID;
}  // namespace Tags

using MapID = nenya::strong_type<int, Tags::MapID>;

}  // namespace Tactile

#pragma once

#include <nenya.hpp>  // strong_type

namespace tactile {

namespace tags {
struct tile_id_tag;
}  // namespace tags

using tile_id = nenya::strong_type<int, tags::tile_id_tag>;

}  // namespace tactile

#pragma once

#include <nenya.hpp>  // strong_type

namespace tactile {

namespace tags {
struct tileset_id_tag;
}  // namespace tags

using tileset_id = nenya::strong_type<int, tags::tileset_id_tag>;

}  // namespace tactile

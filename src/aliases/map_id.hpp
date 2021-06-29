#pragma once

#include <nenya.hpp>  // strong_type

namespace Tactile {

namespace tags {
struct map_id_tag;
}  // namespace tags

using map_id = nenya::strong_type<int, tags::map_id_tag>;

}  // namespace Tactile

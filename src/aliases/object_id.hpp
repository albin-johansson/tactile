#pragma once

#include <nenya.hpp>  // strong_type

namespace tactile {

namespace tags {
struct object_id_tag;
}  // namespace tags

using object_id = nenya::strong_type<int, tags::object_id_tag>;

}  // namespace tactile

#pragma once

#include <nenya.hpp>  // strong_type

namespace Tactile {

namespace tags {
struct object_ref_tag;
}  // namespace tags

using object_ref = nenya::strong_type<int, tags::object_ref_tag>;

}  // namespace Tactile

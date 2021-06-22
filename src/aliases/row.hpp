#pragma once

#include <nenya.hpp>  // strong_type

namespace tactile {

namespace tags {
struct row_tag;
}  // namespace tags

using row_t = nenya::strong_type<int, tags::row_tag>;

}  // namespace tactile

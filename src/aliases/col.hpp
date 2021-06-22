#pragma once

#include <nenya.hpp>  // strong_type

namespace tactile {

namespace tags {
struct col_tag;
}  // namespace tags

using col_t = nenya::strong_type<int, tags::col_tag>;

}  // namespace tactile

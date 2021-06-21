#pragma once

#include <nenya.hpp>  // strong_type

namespace tactile {

namespace tags {
struct layer_id_tag;
}  // namespace tags

using layer_id = nenya::strong_type<int, tags::layer_id_tag>;

}  // namespace tactile

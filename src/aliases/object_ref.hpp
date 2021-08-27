#pragma once

#include <nenya.hpp>  // strong_type

namespace Tactile {

namespace Tags {
struct ObjectRefTag;
}  // namespace Tags

using object_ref = nenya::strong_type<int, Tags::ObjectRefTag>;

}  // namespace Tactile

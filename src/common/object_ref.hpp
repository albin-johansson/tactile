#pragma once

#include <nenya.hpp>  // strong_type

namespace Tactile {

namespace Tags {
struct ObjectRef;
}  // namespace Tags

using ObjectRef = nenya::strong_type<int, Tags::ObjectRef>;

}  // namespace Tactile

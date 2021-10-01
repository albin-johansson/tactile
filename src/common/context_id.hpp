#pragma once

#include <nenya.hpp>  // strong_type

namespace Tactile {

namespace Tags {
struct ContextID;
}  // namespace Tags

// General ID used to identify different property contexts (these IDs are not saved)
using ContextID = nenya::strong_type<int, Tags::ContextID>;

}  // namespace Tactile

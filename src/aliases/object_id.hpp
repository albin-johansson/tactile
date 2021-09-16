#pragma once

#include <nenya.hpp>  // strong_type

#include "ints.hpp"

namespace Tactile {

namespace Tags {
struct ObjectID;
}  // namespace Tags

using ObjectID = nenya::strong_type<int, Tags::ObjectID>;

[[nodiscard]] constexpr auto operator""_obj(const ulonglong value) noexcept -> ObjectID
{
  return ObjectID{static_cast<ObjectID::value_type>(value)};
}

}  // namespace Tactile

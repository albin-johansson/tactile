// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "object_type.hpp"

#include <magic_enum.hpp>

namespace tactile {

auto operator<<(std::ostream& stream, const ObjectType type) -> std::ostream&
{
  return stream << magic_enum::enum_name(type);
}

}  // namespace tactile

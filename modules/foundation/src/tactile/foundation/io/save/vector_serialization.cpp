// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/save/vector_serialization.hpp"

#include <fmt/format.h>

namespace tactile {

auto serialize(const Int2& vec) -> String
{
  return fmt::format("{};{}", vec.x, vec.y);
}

auto serialize(const Int3& vec) -> String
{
  return fmt::format("{};{};{}", vec.x, vec.y, vec.z);
}

auto serialize(const Int4& vec) -> String
{
  return fmt::format("{};{};{};{}", vec.x, vec.y, vec.z, vec.w);
}

auto serialize(const Float2& vec) -> String
{
  return fmt::format("{};{}", vec.x, vec.y);
}

auto serialize(const Float3& vec) -> String
{
  return fmt::format("{};{};{}", vec.x, vec.y, vec.z);
}

auto serialize(const Float4& vec) -> String
{
  return fmt::format("{};{};{};{}", vec.x, vec.y, vec.z, vec.w);
}

}  // namespace tactile

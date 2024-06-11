// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/entity/entity.hpp"

#include <format>  // format

namespace tactile {

auto entity_to_string(const EntityID entity) -> String
{
  return std::format("({:X},v{})",
                     entt::to_entity(entity),
                     entt::to_version(entity));
}

}  // namespace tactile

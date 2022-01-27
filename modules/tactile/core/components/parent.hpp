#pragma once

#include <vector>  // vector

#include <entt/entt.hpp>  // entity, null

namespace tactile::comp {

struct parent final
{
  entt::entity entity{entt::null};
};

}  // namespace tactile::comp

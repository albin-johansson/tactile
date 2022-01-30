#pragma once

#include <string>  // string
#include <vector>  // vector

#include <entt/entt.hpp>

#include "tactile_def.hpp"

namespace tactile::comp {

struct attribute_context final
{
  context_id id{};
  std::string name;
  std::vector<entt::entity> properties;
  std::vector<entt::entity> components;
};

struct active_attribute_context final
{
  entt::entity entity{entt::null};
};

}  // namespace tactile::comp

#pragma once

#include <string>  // string
#include <vector>  // vector

#include <entt/entt.hpp>  // entity
#include <tactile_def.hpp>

namespace tactile {

struct PropertyContext final
{
  ContextID id{};
  std::string name;
  std::vector<entt::entity> properties;
  std::vector<entt::entity> components;
};

struct ActivePropertyContext final
{
  entt::entity entity{entt::null};
};

}  // namespace tactile

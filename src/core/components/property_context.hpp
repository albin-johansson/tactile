#pragma once

#include <entt.hpp>  // entity
#include <string>    // string
#include <vector>    // vector

namespace Tactile {

struct PropertyContext final
{
  std::string name;
  std::vector<entt::entity> properties;
};

struct ActivePropertyContext final
{
  entt::entity entity{entt::null};
};

}  // namespace Tactile

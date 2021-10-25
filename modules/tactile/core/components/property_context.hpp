#pragma once

#include <string>  // string
#include <vector>  // vector

#include <entt.hpp>  // entity

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

struct PropertyContext final
{
  ContextID id;
  std::string name;
  std::vector<entt::entity> properties;
};

struct ActivePropertyContext final
{
  entt::entity entity{entt::null};
};

}  // namespace Tactile

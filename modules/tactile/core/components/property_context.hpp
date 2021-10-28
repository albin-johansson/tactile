#pragma once

#include <string>  // string
#include <vector>  // vector

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // entity

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

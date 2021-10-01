#pragma once

#include <entt.hpp>  // entity
#include <string>    // string
#include <vector>    // vector

#include "common/context_id.hpp"

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

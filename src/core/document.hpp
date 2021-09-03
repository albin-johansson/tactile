#pragma once

#include <entt.hpp>    // registry
#include <filesystem>  // path

#include "commands/command_stack.hpp"

namespace Tactile {

struct Document final
{
  entt::registry registry;
  CommandStack commands;
  std::filesystem::path path;
};

}  // namespace Tactile
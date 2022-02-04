#pragma once

#include <filesystem>  // path

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_stack.hpp"

namespace tactile {

struct Document final
{
  entt::registry registry;
  command_stack commands;
  std::filesystem::path path;
};

}  // namespace tactile
#pragma once

#include <filesystem>  // path

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_stack.hpp"

namespace Tactile {

struct Document final {
  entt::registry registry;
  CommandStack commands;
  std::filesystem::path path;
};

}  // namespace Tactile
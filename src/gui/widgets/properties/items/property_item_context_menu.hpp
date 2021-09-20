#pragma once

#include <entt.hpp>  // dispatcher
#include <string>    // string

namespace Tactile {

struct PropertyItemContextMenuState final
{
  bool show_add_dialog{};
  bool show_rename_dialog{};
  bool show_change_type_dialog{};
};

void PropertyItemContextMenu(entt::dispatcher& dispatcher,
                             const std::string& name,
                             PropertyItemContextMenuState& state);

}  // namespace Tactile

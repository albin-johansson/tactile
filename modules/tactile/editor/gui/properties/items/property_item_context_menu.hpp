#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // dispatcher

namespace tactile {

struct PropertyItemContextMenuState final
{
  bool show_add_dialog{};
  bool show_rename_dialog{};
  bool show_change_type_dialog{};
};

auto PropertyItemContextMenu(entt::dispatcher& dispatcher,
                             const std::string& name,
                             PropertyItemContextMenuState& state) -> bool;

}  // namespace tactile

#pragma once

#include <centurion.hpp>
#include <entt/entt.hpp>

namespace tactile {

class Model;
class widget_manager;

void LoadDefaultShortcuts();

void UpdateShortcuts(const Model& model,
                     const widget_manager& widgets,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher);

}  // namespace tactile

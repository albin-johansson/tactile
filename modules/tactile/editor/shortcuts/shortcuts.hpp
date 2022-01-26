#pragma once

#include <centurion.hpp>
#include <entt/entt.hpp>

namespace tactile {

class Model;
class WidgetManager;

void LoadDefaultShortcuts();

void UpdateShortcuts(const Model& model,
                     const WidgetManager& widgets,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher);

}  // namespace tactile

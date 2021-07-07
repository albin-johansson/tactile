#pragma once

#include <centurion.hpp>  // keyboard_event
#include <entt.hpp>       // dispatcher

namespace Tactile {

class Model;

void UpdateShortcuts(const Model& model,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher);

}  // namespace Tactile

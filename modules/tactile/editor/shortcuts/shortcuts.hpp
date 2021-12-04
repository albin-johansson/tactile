#pragma once

#include <SDL.h>
#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;
class WidgetManager;

void LoadDefaultShortcuts();

void UpdateShortcuts(const Model& model,
                     const WidgetManager& widgets,
                     const SDL_KeyboardEvent& event,
                     entt::dispatcher& dispatcher);

}  // namespace Tactile

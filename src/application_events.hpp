#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Application;

void SubscribeToEvents(Application* app, entt::dispatcher& dispatcher);

}  // namespace Tactile

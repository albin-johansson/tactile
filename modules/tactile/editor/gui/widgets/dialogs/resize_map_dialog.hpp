#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

void UpdateResizeMapDialog(entt::dispatcher& dispatcher);

void OpenResizeMapDialog(int currentRows, int currentColumns);

}  // namespace Tactile

#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

void UpdateResizeMapDialog(entt::dispatcher& dispatcher);

void OpenResizeMapDialog(usize currentRows, usize currentColumns);

}  // namespace Tactile

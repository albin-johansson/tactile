#pragma once

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // dispatcher

namespace Tactile {

void UpdateResizeMapDialog(entt::dispatcher& dispatcher);

void OpenResizeMapDialog(usize currentRows, usize currentColumns);

}  // namespace Tactile

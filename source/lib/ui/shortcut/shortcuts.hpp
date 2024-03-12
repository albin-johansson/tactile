// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/fwd.hpp>
#include <entt/signal/fwd.hpp>

namespace tactile {

/// Initializes the default shortcut bindings, this should be called once at startup.
void init_default_shortcuts();

/// Polls all shortcuts after a keyboard event.
void update_shortcuts(const cen::keyboard_event& event, entt::dispatcher& dispatcher);

}  // namespace tactile

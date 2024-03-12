// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/keyboard.hpp>
#include <entt/signal/fwd.hpp>

#include "tactile/base/container/function.hpp"
#include "ui/menu/menu.hpp"

namespace tactile {

struct Shortcut final {
  MenuAction action;
  cen::scan_code key;
  cen::key_mod modifiers {cen::key_mod::none};
  Function<void(entt::dispatcher&)> activate;
};

}  // namespace tactile

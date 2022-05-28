/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <centurion/event.hpp>
#include <centurion/keyboard.hpp>
#include <entt/fwd.hpp>

#include "core/fwd.hpp"
#include "editor/fwd.hpp"

namespace tactile {

// TODO
// struct Shortcut final
// {
//   void (*trigger)(entt::dispatcher&) = nullptr;
//   bool (*is_enabled)(const DocumentModel&) = nullptr;
//   cen::scan_code key;
//   cen::key_mod mods{cen::key_mod::none};
// };

class AShortcut
{
 public:
  explicit AShortcut(const cen::scan_code key,
                     const cen::key_mod modifiers = cen::key_mod::none)
      : mKey{key}
      , mModifiers{modifiers}
  {}

  virtual ~AShortcut() noexcept = default;

  void poll(const DocumentModel& model,
            const cen::keyboard_event& event,
            entt::dispatcher& dispatcher);

  virtual void activate(entt::dispatcher& dispatcher) = 0;

  [[nodiscard]] virtual auto is_enabled([[maybe_unused]] const DocumentModel& model) const
      -> bool
  {
    return true;
  }

 private:
  cen::scan_code mKey;
  cen::key_mod mModifiers{cen::key_mod::none};
};

}  // namespace tactile

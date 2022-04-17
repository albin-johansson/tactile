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

#include <centurion.hpp>
#include <entt/fwd.hpp>

#include "editor/fwd.hpp"
#include "tactile.hpp"

namespace tactile {

class WidgetManager;

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
            const WidgetManager& widgets,
            const cen::keyboard_event& event,
            entt::dispatcher& dispatcher);

  virtual void activate(entt::dispatcher& dispatcher) = 0;

  [[nodiscard]] virtual auto is_enabled(
      [[maybe_unused]] const DocumentModel& model,
      [[maybe_unused]] const WidgetManager& widgets) const -> bool
  {
    return true;
  }

 private:
  cen::scan_code mKey;
  cen::key_mod mModifiers{cen::key_mod::none};
};

}  // namespace tactile

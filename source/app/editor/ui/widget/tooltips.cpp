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

#include "tooltips.hpp"

#include <chrono>  // system_clock

#include <imgui.h>

#include "core/type/hash_map.hpp"
#include "core/type/maybe.hpp"
#include "core/util/assoc.hpp"
#include "misc/assert.hpp"

using namespace std::chrono_literals;

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point;

namespace tactile::ui {

void lazy_tooltip(const char* id, const char* tooltip)
{
  TACTILE_ASSERT(id);
  TACTILE_ASSERT(tooltip);

  static HashMap<ImGuiID, Maybe<TimePoint>> state;

  const auto hashed_id = ImGui::GetID(id);
  auto& last_hover = state[hashed_id];

  if (ImGui::IsItemHovered()) {
    if (!last_hover) {
      last_hover = Clock::now();
    }

    if (Clock::now() - last_hover.value() > 1s) {
      ImGui::SetTooltip("%s", tooltip);
    }
  }
  else {
    last_hover.reset();
  }
}

}  // namespace tactile::ui
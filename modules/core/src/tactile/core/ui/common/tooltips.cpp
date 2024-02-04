// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/tooltips.hpp"

#include <imgui.h>

#include "tactile/foundation/container/hash_map.hpp"
#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/misc/chrono.hpp"

using namespace std::chrono_literals;

namespace tactile::core {

void lazy_tooltip(const char* id, const char* text)
{
  TACTILE_ASSERT(id);
  TACTILE_ASSERT(text);

  static HashMap<ImGuiID, Maybe<SystemClockInstant>> state;

  const auto hashed_id = ImGui::GetID(id);
  auto& last_hover_time = state[hashed_id];

  if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
    if (!last_hover_time.has_value()) {
      last_hover_time = SystemClock::now();
    }

    if (SystemClock::now() - last_hover_time.value() > 1s) {
      ImGui::SetTooltip("%s", text);  // NOLINT(*-pro-type-vararg)
    }
  }
  else {
    last_hover_time.reset();
  }
}

}  // namespace tactile::core

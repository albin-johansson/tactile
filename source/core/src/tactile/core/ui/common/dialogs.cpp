// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/dialogs.hpp"

#include <imgui.h>

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/ui/common/buttons.hpp"

namespace tactile::ui {

auto push_dialog_control_buttons(const char* close,
                                 const char* accept,
                                 const char* apply,
                                 const bool can_accept,
                                 const bool can_apply) -> DialogStatus
{
  TACTILE_ASSERT(close != nullptr);
  TACTILE_ASSERT(accept != nullptr);

  auto status = DialogStatus::kIdle;

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  if (push_button(close)) {
    status = DialogStatus::kClosed;
    ImGui::CloseCurrentPopup();
  }

  ImGui::SameLine();

  if (push_button(accept, nullptr, can_accept)) {
    status = DialogStatus::kAccepted;
    ImGui::CloseCurrentPopup();
  }

  if (apply != nullptr) {
    ImGui::SameLine();

    if (push_button(apply, nullptr, can_apply)) {
      status = DialogStatus::kApplied;
    }
  }

  return status;
}

}  // namespace tactile::ui

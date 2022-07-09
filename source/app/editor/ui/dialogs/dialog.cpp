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

#include "editor/ui/dialogs/dialog.hpp"

#include "editor/ui/alignment.hpp"
#include "editor/ui/common/buttons.hpp"
#include "editor/ui/scoped.hpp"
#include "misc/panic.hpp"

namespace tactile::ui {

ADialog::ADialog(const char* title) : mTitle {title}
{
  if (!mTitle) {
    throw TactileError("Invalid null dialog title!");
  }
}

void ADialog::update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (mShow) {
    ImGui::OpenPopup(mTitle);
    mShow = false;
  }

  center_next_window_on_appearance();

  constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (Modal modal {mTitle, flags}; modal.is_open()) {
    on_update(model, dispatcher);

    ImGui::Spacing();

    if (mCloseButtonLabel && button(mCloseButtonLabel)) {
      on_cancel();
      ImGui::CloseCurrentPopup();
    }

    const auto valid = is_current_input_valid(model);

    if (mAcceptButtonLabel) {
      if (mCloseButtonLabel) {
        ImGui::SameLine();
      }

      if (button(mAcceptButtonLabel, nullptr, valid)) {
        on_accept(dispatcher);
        ImGui::CloseCurrentPopup();
      }
    }

    if (mUseApplyButton) {
      if (mCloseButtonLabel || mAcceptButtonLabel) {
        ImGui::SameLine();
      }
      if (button("Apply", nullptr, valid)) {
        on_apply(dispatcher);
      }
    }
  }
}

void ADialog::make_visible()
{
  mShow = true;
}

void ADialog::use_apply_button()
{
  mUseApplyButton = true;
}

void ADialog::set_accept_button_label(const char* label)
{
  mAcceptButtonLabel = label;
}

void ADialog::set_close_button_label(const char* label)
{
  mCloseButtonLabel = label;
}

}  // namespace tactile::ui
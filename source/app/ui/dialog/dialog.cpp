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

#include "dialog.hpp"

#include "ui/style/alignment.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

[[nodiscard]] auto ui_dialog_begin(const char* title,
                                   bool* open = nullptr,
                                   const uint32 flags = UI_DIALOG_FLAG_NONE) -> bool
{
  if (flags & UI_DIALOG_FLAG_OPEN) {
    ImGui::OpenPopup(title);
  }

  center_next_window_on_appearance();

  const ImGuiWindowFlags popup_flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  return ImGui::BeginPopupModal(title, open, popup_flags);
}

[[nodiscard]] auto ui_dialog_end(const char* close_label,
                                 const char* accept_label = nullptr,
                                 const char* apply_label = nullptr,
                                 const uint32 flags = UI_DIALOG_FLAG_NONE) -> DialogAction
{
  ImGui::Spacing();

  auto action = DialogAction::None;

  if (close_label && ui_button(close_label)) {
    action = DialogAction::Cancel;
    ImGui::CloseCurrentPopup();
  }

  const bool is_input_valid = flags & UI_DIALOG_FLAG_INPUT_IS_VALID;

  if (accept_label) {
    if (close_label) {
      ImGui::SameLine();
    }

    if (ui_button(accept_label, nullptr, is_input_valid)) {
      action = DialogAction::Accept;
      ImGui::CloseCurrentPopup();
    }
  }

  if (apply_label) {
    if (close_label || accept_label) {
      ImGui::SameLine();
    }

    if (ui_button(apply_label, nullptr, is_input_valid)) {
      action = DialogAction::Apply;
    }
  }

  ImGui::EndPopup();
  return action;
}

}  // namespace

ScopedDialog::ScopedDialog(const DialogOptions& options, DialogAction* action)
    : mOptions {options},
      mAction {action},
      mWasOpened {ui_dialog_begin(options.title, options.is_open, options.flags)}
{
}

ScopedDialog::~ScopedDialog()
{
  if (mWasOpened) {
    const auto action = ui_dialog_end(mOptions.close_label,
                                      mOptions.accept_label,
                                      mOptions.apply_label,
                                      mOptions.flags);
    if (mAction) {
      *mAction = action;
    }
  }
}

}  // namespace tactile::ui
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

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/buttons.hpp"
#include "ui/widget/scoped.hpp"

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

Dialog::Dialog(String title)
    : mTitle {std::move(title)},
      mAcceptButtonLabel {get_current_language().misc.ok},
      mCloseButtonLabel {get_current_language().misc.cancel}
{
}

void Dialog::update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (mShow) {
    ImGui::OpenPopup(mTitle.c_str());
    mShow = false;
  }

  center_next_window_on_appearance();

  constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (const Modal modal {mTitle.c_str(), flags}; modal.is_open()) {
    on_update(model, dispatcher);

    ImGui::Spacing();

    if (mCloseButtonLabel && ui_button(mCloseButtonLabel->c_str())) {
      on_cancel();
      ImGui::CloseCurrentPopup();
    }

    const auto valid = is_current_input_valid(model);

    if (mAcceptButtonLabel) {
      if (mCloseButtonLabel) {
        ImGui::SameLine();
      }

      if (ui_button(mAcceptButtonLabel->c_str(), nullptr, valid)) {
        on_accept(dispatcher);
        ImGui::CloseCurrentPopup();
      }
    }

    if (mUseApplyButton) {
      if (mCloseButtonLabel || mAcceptButtonLabel) {
        ImGui::SameLine();
      }
      if (ui_button(get_current_language().misc.apply.c_str(), nullptr, valid)) {
        on_apply(dispatcher);
      }
    }
  }
}

void Dialog::make_visible()
{
  mShow = true;
}

void Dialog::use_apply_button()
{
  mUseApplyButton = true;
}

void Dialog::set_title(String title)
{
  mTitle = std::move(title);
}

void Dialog::set_accept_button_label(Maybe<String> label)
{
  mAcceptButtonLabel = std::move(label);
}

void Dialog::set_close_button_label(Maybe<String> label)
{
  mCloseButtonLabel = std::move(label);
}

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
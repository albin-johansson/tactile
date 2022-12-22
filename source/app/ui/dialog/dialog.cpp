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
  if (Modal modal {mTitle.c_str(), flags}; modal.is_open()) {
    on_update(model, dispatcher);

    ImGui::Spacing();

    if (mCloseButtonLabel && button(mCloseButtonLabel->c_str())) {
      on_cancel();
      ImGui::CloseCurrentPopup();
    }

    const auto valid = is_current_input_valid(model);

    if (mAcceptButtonLabel) {
      if (mCloseButtonLabel) {
        ImGui::SameLine();
      }

      if (button(mAcceptButtonLabel->c_str(), nullptr, valid)) {
        on_accept(dispatcher);
        ImGui::CloseCurrentPopup();
      }
    }

    if (mUseApplyButton) {
      if (mCloseButtonLabel || mAcceptButtonLabel) {
        ImGui::SameLine();
      }
      if (button(get_current_language().misc.apply.c_str(), nullptr, valid)) {
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

}  // namespace tactile::ui
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

#include "string_input_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "core/util/buffers.hpp"

namespace tactile::ui {

StringInputDialog::StringInputDialog(std::string title)
    : Dialog {std::move(title)}
{
}

void StringInputDialog::show(std::string previous)
{
  mShouldAcquireFocus = true;
  mPrevious = std::move(previous);
  copy_string_into_buffer(mBuffer, mPrevious);
  make_visible();
}

void StringInputDialog::set_input_hint(Maybe<std::string> hint)
{
  mHint = std::move(hint);
}

void StringInputDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  if (mShouldAcquireFocus) {
    ImGui::SetKeyboardFocusHere();
    mShouldAcquireFocus = false;
  }
  ImGui::InputTextWithHint("##string_input_dialog_input",
                           mHint.has_value() ? mHint->c_str() : "",
                           mBuffer.data(),
                           sizeof mBuffer);
}

auto StringInputDialog::is_current_input_valid(const DocumentModel& model) const -> bool
{
  return validate(model, current_input());
}

auto StringInputDialog::current_input() const -> std::string_view
{
  return create_string_view_from_buffer(mBuffer);
}

auto StringInputDialog::previous_input() const -> const std::string&
{
  return mPrevious;
}

}  // namespace tactile::ui

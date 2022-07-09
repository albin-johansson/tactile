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

#include "dialogs.hpp"

#include <utility>  // move

#include "editor/ui/shared/dialog_state.hpp"
#include "misc/panic.hpp"

namespace tactile::ui {
namespace {

inline Unique<DialogState> _dialogs;

}  // namespace

void init_dialogs()
{
  _dialogs = std::make_unique<DialogState>();
}

void set_dialog_state(Unique<DialogState> state)
{
  _dialogs = std::move(state);
}

auto get_dialogs() -> DialogState&
{
  if (_dialogs) [[likely]] {
    return *_dialogs;
  }
  else {
    throw TactileError {"No available dialog state!"};
  }
}

}  // namespace tactile::ui
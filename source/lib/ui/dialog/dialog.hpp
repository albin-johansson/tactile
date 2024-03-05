/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/numeric.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::ui {

enum DialogFlags : uint32 {
  UI_DIALOG_FLAG_NONE = 0,
  UI_DIALOG_FLAG_OPEN = 1u << 1u,
  UI_DIALOG_FLAG_INPUT_IS_VALID = 1 << 2u,
};

enum class DialogAction {
  None,
  Cancel,
  Accept,
  Apply
};

struct DialogOptions final {
  const char* title {};
  const char* close_label {};
  const char* accept_label {};
  const char* apply_label {};
  bool* is_open {};
  uint32 flags {UI_DIALOG_FLAG_NONE};
};

class ScopedDialog final {
 public:
  TACTILE_DELETE_COPY(ScopedDialog);
  TACTILE_DELETE_MOVE(ScopedDialog);

  explicit ScopedDialog(const DialogOptions& options, DialogAction* action = nullptr);

  ~ScopedDialog();

  [[nodiscard]] auto was_opened() const -> bool { return mWasOpened; }

 private:
  DialogOptions mOptions;
  DialogAction* mAction;
  bool mWasOpened {};
};

}  // namespace tactile::ui

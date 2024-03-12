// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
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

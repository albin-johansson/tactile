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

#pragma once

#include <entt/fwd.hpp>

#include "core/type/maybe.hpp"
#include "core/type/string.hpp"
#include "core/vocabulary.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)

namespace tactile::ui {

/// The base class of all modal dialogs.
class Dialog {
 public:
  TACTILE_DEFAULT_COPY(Dialog);
  TACTILE_DEFAULT_MOVE(Dialog);

  explicit Dialog(String title);

  virtual ~Dialog() noexcept = default;

  /// Updates the state of the dialog.
  void update(const DocumentModel& model, entt::dispatcher& dispatcher);

 protected:
  /// Updates dialog specific components.
  /// Do not provide widgets such as accept and close buttons in this function.
  virtual void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) = 0;

  /// Invoked when the "Cancel" button is pressed.
  virtual void on_cancel() {}

  /// Invoked when the "OK" (accept) button is pressed.
  virtual void on_accept([[maybe_unused]] entt::dispatcher& dispatcher) {}

  /// Invoked when the "Apply" (secondary accept) button is pressed.
  virtual void on_apply([[maybe_unused]] entt::dispatcher& dispatcher) {}

  /// Indicates whether the current input state is considered acceptable.
  [[nodiscard]] virtual auto is_current_input_valid(
      [[maybe_unused]] const DocumentModel& model) const -> bool
  {
    return true;
  }

  /// Makes the dialog visible.
  ///
  /// This is meant to be used by derived classes to actually display the dialog.
  /// This function is protected since not all dialogs supports being shown with no
  /// initial arguments.
  void make_visible();

  /// Includes an "Apply" button.
  /// By default, the apply button is not shown.
  void use_apply_button();

  /// Sets the title of the dialog, this must be set before the dialog is updated.
  void set_title(String title);

  /// Sets the label of the "accept" button.
  ///
  /// By default, the accept button uses "OK" as its label. However, it is often
  /// more intuitive to use more descriptive verbs as the label.
  ///
  /// \param label the label text, use an empty optional to hide the button.
  void set_accept_button_label(Maybe<String> label);

  /// Sets the label of the "close" button.
  ///
  /// By default, the accept button uses "Cancel" as its label.
  ///
  /// \param label the label text, use an empty optional to hide the button.
  void set_close_button_label(Maybe<String> label);

 private:
  String mTitle;
  Maybe<String> mAcceptButtonLabel;
  Maybe<String> mCloseButtonLabel;
  bool mUseApplyButton : 1 {};
  bool mShow           : 1 {};
};

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

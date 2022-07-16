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

#include "core/common/macros.hpp"

namespace tactile {
class DocumentModel;
}  // namespace tactile

namespace tactile::ui {

/**
 * Serves as the base implementation of all modal dialogs.
 */
class ADialog
{
 public:
  TACTILE_DEFAULT_COPY(ADialog);
  TACTILE_DEFAULT_MOVE(ADialog);

  /**
   * Creates a dialog.
   *
   * \param title the non-null title of the dialog.
   *
   * \throws TactileError if the title is null.
   */
  explicit ADialog(const char* title);

  virtual ~ADialog() noexcept = default;

  /**
   * Updates the state of the dialog.
   *
   * \param model the associated model.
   * \param dispatcher the event dispatcher used.
   */
  void update(const DocumentModel& model, entt::dispatcher& dispatcher);

 protected:
  /**
   * Updates dialog specific components.
   *
   * Do not provide components such as accept and close buttons in this function.
   *
   * \param model the associated model.
   * \param dispatcher the event dispatcher used.
   */
  virtual void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) = 0;

  /**
   * Invoked when the "Cancel" button is pressed.
   */
  virtual void on_cancel()
  {}

  /**
   * Invoked when the "OK" (accept) button is pressed.
   *
   * \param dispatcher the event dispatcher used.
   */
  virtual void on_accept([[maybe_unused]] entt::dispatcher& dispatcher)
  {}

  /**
   * Invoked when the "Apply" (secondary accept) button is pressed.
   *
   * \param dispatcher the event dispatcher used.
   */
  virtual void on_apply([[maybe_unused]] entt::dispatcher& dispatcher)
  {}

  /**
   * Indicates whether the current input state is considered acceptable.
   *
   * \param model the associated model.
   *
   * \return `true` if the state of the input is acceptable; `false` otherwise.
   */
  [[nodiscard]] virtual auto is_current_input_valid(
      [[maybe_unused]] const DocumentModel& model) const -> bool
  {
    return true;
  }

  /**
   * Makes the dialog visible.
   *
   * This is meant to be used by derived classes to actually display the dialog.
   * This function is protected since not all dialogs supports being shown with no initial
   * arguments.
   */
  void make_visible();

  /**
   * Includes an "Apply" button.
   *
   * By default, the apply button is not shown.
   *
   * \see on_apply()
   */
  void use_apply_button();

  /**
   * Sets the label of the "accept" button.
   *
   * By default, the accept button uses "OK" as its label. However, it is often
   * more intuitive to use more descriptive verbs as the label, e.g. "Create", "Rename",
   * etc.
   *
   * \param label the label text;
   *              a null pointer may be used to indicate that the button will be hidden.
   */
  void set_accept_button_label(const char* label);

  /**
   * Sets the label of the "close" button.
   *
   * By default, the accept button uses "Cancel" as its label.
   *
   * \param label the label text;
   *              a null pointer may be used to indicate that the button will be hidden.
   */
  void set_close_button_label(const char* label);

 private:
  const char* mTitle {};
  const char* mAcceptButtonLabel {"OK"};
  const char* mCloseButtonLabel {"Cancel"};
  bool        mUseApplyButton : 1 {};
  bool        mShow           : 1 {};
};

}  // namespace tactile::ui

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

#include <array>        // array
#include <string>       // string
#include <string_view>  // string_view

#include <entt/fwd.hpp>

#include "editor/ui/dialogs/dialog.hpp"

namespace tactile::ui {

/**
 * An abstract class representing dialogs which provide a string input field.
 */
class AStringInputDialog : public ADialog
{
 public:
  explicit AStringInputDialog(const char* title);

  void show(std::string previous);

 protected:
  /**
   * Sets the input hint used by the string field.
   *
   * \param hint optional hint label; a null pointer disables the input hint.
   */
  void set_input_hint(const char* hint);

  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) final;

  [[nodiscard]] auto is_current_input_valid(const DocumentModel& model) const
      -> bool final;

  /**
   * Validates an input string.
   *
   * \param model the associated model instance.
   * \param input the current input string.
   *
   * \return `true` if the input string is valid; `false` otherwise.
   */
  [[nodiscard]] virtual auto validate(const DocumentModel& model,
                                      std::string_view     input) const -> bool = 0;

  /**
   * Returns the current string input.
   *
   * \return a view into the current input.
   */
  [[nodiscard]] auto current_input() const -> std::string_view;

  /**
   * Returns the initial input string.
   *
   * \return the initial input.
   */
  [[nodiscard]] auto previous_input() const -> const std::string&;

 private:
  const char*           mHint {};
  std::string           mPrevious;
  std::array<char, 128> mBuffer {};
  bool                  mShouldAcquireFocus {};
};

}  // namespace tactile::ui
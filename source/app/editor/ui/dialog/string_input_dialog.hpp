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

#include "core/common/maybe.hpp"
#include "editor/ui/dialog/dialog.hpp"

namespace tactile::ui {

/// Abstract class representing dialogs which provide a string input field.
class AStringInputDialog : public Dialog {
 public:
  explicit AStringInputDialog(std::string title);

  void show(std::string previous);

 protected:
  void set_input_hint(Maybe<std::string> hint);

  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) final;

  [[nodiscard]] auto is_current_input_valid(const DocumentModel& model) const
      -> bool final;

  /// Validates an input string.
  [[nodiscard]] virtual auto validate(const DocumentModel& model,
                                      std::string_view input) const -> bool = 0;

  [[nodiscard]] auto current_input() const -> std::string_view;

  [[nodiscard]] auto previous_input() const -> const std::string&;

 private:
  Maybe<std::string> mHint;
  std::string mPrevious;
  std::array<char, 128> mBuffer {};
  bool mShouldAcquireFocus {};
};

}  // namespace tactile::ui
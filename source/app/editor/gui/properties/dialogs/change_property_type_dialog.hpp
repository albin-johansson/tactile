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

#include <string>  // string

#include "core/attribute.hpp"
#include "editor/gui/dialogs/dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class ChangePropertyTypeDialog final : public dialog_base
{
 public:
  ChangePropertyTypeDialog();

  ~ChangePropertyTypeDialog() override = default;

  void Show(std::string name, attribute_type type);

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_current_input_valid(const document_model& model) const
      -> bool override;

 private:
  attribute_type mCurrentType{attribute_type::string};
  maybe<std::string> mPropertyName;
  maybe<attribute_type> mPreviousType;
};

}  // namespace tactile

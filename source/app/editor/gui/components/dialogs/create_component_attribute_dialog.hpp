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

#include "core/components/attributes.hpp"
#include "editor/gui/dialogs/string_input_dialog.hpp"

namespace tactile {

class create_component_attribute_dialog final : public string_input_dialog
{
 public:
  create_component_attribute_dialog();

  void show(component_id id);

 protected:
  [[nodiscard]] auto validate(const document_model& model, std::string_view input) const
      -> bool override;

  void on_accept(entt::dispatcher& dispatcher) override;

 private:
  component_id mComponentId{};
};

}  // namespace tactile

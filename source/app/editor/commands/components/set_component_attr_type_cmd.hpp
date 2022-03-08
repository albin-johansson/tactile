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

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class set_component_attr_type_cmd final : public command_base
{
 public:
  set_component_attr_type_cmd(registry_ref registry,
                              component_id id,
                              std::string attribute,
                              attribute_type type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::set_component_attribute_type;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId;
  std::string mAttributeName;
  attribute_type mNewType;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile

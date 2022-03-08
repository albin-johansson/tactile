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

#include "component_name_dialog.hpp"
#include "core/components/component.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Used to change the name of a component definition.
 *
 * \ingroup gui
 */
class rename_component_dialog final : public component_name_dialog
{
 public:
  rename_component_dialog();

  void show(std::string previousName, component_id id);

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;

 private:
  maybe<component_id> mComponentId;
};

}  // namespace tactile

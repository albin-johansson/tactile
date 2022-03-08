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

#include <memory>  // unique_ptr

#include <entt/entt.hpp>

#include "core/attribute.hpp"
#include "editor/gui/dialogs/dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class document_model;

/**
 * \brief Used to manage the available component definitions.
 *
 * \ingroup gui
 */
class component_editor final : public dialog_base
{
 public:
  TACTILE_DELETE_COPY(component_editor)
  TACTILE_DEFAULT_MOVE(component_editor)

  component_editor();

  ~component_editor() noexcept override;

  void show(const document_model& model);

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;

 private:
  struct component_editor_data;
  std::unique_ptr<component_editor_data> mData;

  void show_component_combo_popup(const entt::registry& registry,
                                  entt::dispatcher& dispatcher);

  void show_component_attributes(const entt::registry& registry,
                                 entt::dispatcher& dispatcher,
                                 component_id id);

  void show_component_attribute(entt::dispatcher& dispatcher,
                                component_id id,
                                const std::string& name,
                                const attribute_value& value);
};

}  // namespace tactile

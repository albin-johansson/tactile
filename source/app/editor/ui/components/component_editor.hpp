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
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/fwd.hpp"
#include "editor/fwd.hpp"
#include "editor/ui/dialogs/dialog.hpp"

namespace tactile::ui {

/// Used to manage the available component definitions.
class ComponentEditor final : public ADialog
{
 public:
  TACTILE_DELETE_COPY(ComponentEditor);
  TACTILE_DEFAULT_MOVE(ComponentEditor);

  ComponentEditor();

  ~ComponentEditor() noexcept override;

  void show(const DocumentModel& model);

 protected:
  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) override;

 private:
  struct Data;
  Unique<Data> mData;

  void show_component_combo_popup(const ADocument&  document,
                                  entt::dispatcher& dispatcher);

  void show_component_attributes(const core::ComponentDefinition& definition,
                                 entt::dispatcher&                dispatcher);

  void show_component_attribute(const UUID&        componentId,
                                const std::string& name,
                                const Attribute&   value,
                                entt::dispatcher&  dispatcher);
};

}  // namespace tactile::ui

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

#include <functional>   // function
#include <string>       // string
#include <string_view>  // string_view

#include <boost/uuid/uuid_hash.hpp>

#include "core/attribute.hpp"
#include "core/common/associative.hpp"
#include "core/common/ints.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/comp/component.hpp"
#include "core/context/context.hpp"

namespace tactile {

class ContextManager final
{
  using ComponentFunc = std::function<void(Component&)>;

 public:
  void add_context(Shared<IContext> context);

  void remove_context(const UUID& id);

  [[nodiscard]] auto get_context(const UUID& id) -> const Shared<IContext>&;

  [[nodiscard]] auto view_context(const UUID& id) const -> const IContext&;

  [[nodiscard]] auto has_context(const UUID& id) const -> bool;

  [[nodiscard]] auto context_count() const -> usize;

  auto on_undef_comp(const UUID& componentId) -> HashMap<UUID, Component>;

  void on_new_component_attr(const UUID&        componentId,
                             const std::string& name,
                             const Attribute&   value);

  void on_removed_component_attr(const UUID& componentId, std::string_view name);

  void on_renamed_component_attr(const UUID&        componentId,
                                 std::string_view   oldName,
                                 const std::string& newName);

  auto on_changed_component_attr_type(const UUID&      componentId,
                                      std::string_view name,
                                      AttributeType    type) -> HashMap<UUID, Attribute>;

 private:
  HashMap<UUID, Shared<IContext>> mContexts;

  void on_component_update(const UUID& componentId, const ComponentFunc& func);
};

}  // namespace tactile

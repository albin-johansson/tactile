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

#include <functional>  // function

#include <boost/uuid/uuid_hash.hpp>

#include "core/attribute.hpp"
#include "core/comp/component.hpp"
#include "core/ctx/context.hpp"
#include "core/type/hash_map.hpp"
#include "core/type/ptr.hpp"
#include "core/type/string.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// Manages all of the contexts contained within a document.
class ContextManager final {
  using ContextMap = HashMap<UUID, Shared<Context>>;
  using ComponentFunc = std::function<void(Component&)>;

 public:
  explicit ContextManager(const UUID& rootContextId);

  void add_context(Shared<Context> context);

  void erase(const UUID& contextId);

  void select(const UUID& contextId);

  [[nodiscard]] auto get_context(const UUID& id) -> const Shared<Context>&;

  [[nodiscard]] auto at(const UUID& contextId) -> Context&;
  [[nodiscard]] auto at(const UUID& contextId) const -> const Context&;

  [[nodiscard]] auto contains(const UUID& contextId) const -> bool;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto active_context() -> Context&;
  [[nodiscard]] auto active_context() const -> const Context&;

  auto on_undef_comp(const UUID& componentId) -> HashMap<UUID, Component>;

  void on_new_component_attr(const UUID& componentId,
                             const String& name,
                             const Attribute& value);

  void on_removed_component_attr(const UUID& componentId, StringView name);

  void on_renamed_component_attr(const UUID& componentId,
                                 StringView oldName,
                                 const String& newName);

  auto on_changed_component_attr_type(const UUID& componentId,
                                      StringView name,
                                      AttributeType type) -> HashMap<UUID, Attribute>;

  [[nodiscard]] auto active_context_id() const -> const UUID& { return mActiveContextId; }

 private:
  ContextMap mContexts;
  UUID mRootContextId;
  UUID mActiveContextId;
  // TODO PERFORMANCE: maybe use HashMap<UUID, Vec<Shared<IContext>>>?

  void on_component_update(const UUID& componentId, const ComponentFunc& func);
};

}  // namespace tactile

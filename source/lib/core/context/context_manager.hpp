/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/numeric.hpp"
#include "common/type/func.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/ptr.hpp"
#include "common/type/result.hpp"
#include "common/type/string.hpp"
#include "common/type/uuid.hpp"
#include "core/attribute.hpp"
#include "core/component/component.hpp"
#include "core/context/context.hpp"

namespace tactile {

/// Manages all of the contexts contained within a document.
/// FIXME bad method names
class ContextManager final {
  using ContextMap = HashMap<UUID, Shared<Context>>;
  using ComponentFunc = Func<void(Component&)>;

 public:
  explicit ContextManager(const UUID& root_ctx_id);

  void add_context(Shared<Context> context);

  auto remove_context(const UUID& ctx_id) -> Result;

  auto select_context(const UUID& ctx_id) -> Result;

  [[nodiscard]] auto get_context_ptr(const UUID& ctx_id) -> const Shared<Context>&;

  [[nodiscard]] auto get_context(const UUID& ctx_id) -> Context&;
  [[nodiscard]] auto get_context(const UUID& ctx_id) const -> const Context&;

  [[nodiscard]] auto has_context(const UUID& ctx_id) const -> bool;

  [[nodiscard]] auto context_count() const -> usize;

  [[nodiscard]] auto get_active_context() -> Context&;
  [[nodiscard]] auto get_active_context() const -> const Context&;

  auto on_undef_comp(const UUID& component_id) -> HashMap<UUID, Component>;

  void on_new_component_attr(const UUID& component_id,
                             const String& name,
                             const Attribute& value);

  void on_removed_component_attr(const UUID& component_id, StringView name);

  void on_renamed_component_attr(const UUID& component_id,
                                 StringView old_name,
                                 const String& new_name);

  auto on_changed_component_attr_type(const UUID& component_id,
                                      StringView name,
                                      AttributeType type) -> HashMap<UUID, Attribute>;

  [[nodiscard]] auto active_context_id() const -> const UUID& { return mActiveContextId; }

 private:
  ContextMap mContexts;
  UUID mRootContextId;
  UUID mActiveContextId {};
  // TODO PERFORMANCE: maybe use HashMap<UUID, Vec<Shared<IContext>>>?

  void on_component_update(const UUID& component_id, const ComponentFunc& func);
};

}  // namespace tactile

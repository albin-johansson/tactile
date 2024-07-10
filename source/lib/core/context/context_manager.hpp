// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/component/component.hpp"
#include "core/context/context.hpp"
#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/function.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Manages all of the contexts contained within a document.
/// FIXME bad method names
class ContextManager final {
  using ContextMap = HashMap<UUID, Shared<Context>>;
  using ComponentFunc = Function<void(Component&)>;

 public:
  explicit ContextManager(const UUID& root_ctx_id);

  void add_context(Shared<Context> context);

  auto remove_context(const UUID& ctx_id) -> Result<void>;

  auto select_context(const UUID& ctx_id) -> Result<void>;

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
  // TODO PERFORMANCE: maybe use HashMap<UUID, Vector<Shared<IContext>>>?

  void on_component_update(const UUID& component_id, const ComponentFunc& func);
};

}  // namespace tactile

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/component/component.hpp"
#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/function.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/string_map.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

class ContextInfo final {
 public:
  using ComponentVisitor = Function<void(const UUID&, const Component&)>;
  using PropertyVisitor = Function<void(const String&, const Attribute&)>;

  TACTILE_DEFAULT_COPY(ContextInfo);
  TACTILE_DEFAULT_MOVE(ContextInfo);

  ContextInfo() = default;

  explicit ContextInfo(const UUID& id);

  void set_name(String name);

  /// Visits each property of the context.
  void each_property(const PropertyVisitor& visitor) const;

  auto add_property(String name, Attribute value) -> Result<void>;

  auto add_property(String name, AttributeType type) -> Result<void>;

  auto update_property(StringView name, Attribute value) -> Result<void>;

  auto remove_property(StringView name) -> Maybe<Attribute>;

  auto rename_property(StringView current_name, String new_name) -> Result<void>;

  /// Visits each component attached to the context.
  void each_component(const ComponentVisitor& visitor) const;

  /// Attaches a component to the context.
  /// Has no effect if the context already has an attached component of the same type.
  auto attach_component(Component component) -> Result<void>;

  /// Detaches a component from the context.
  auto detach_component(const UUID& component_id) -> Maybe<Component>;

  [[nodiscard]] auto get_property(StringView name) -> Attribute&;
  [[nodiscard]] auto get_property(StringView name) const -> const Attribute&;
  [[nodiscard]] auto get_component(const UUID& component_id) -> Component&;
  [[nodiscard]] auto get_component(const UUID& component_id) const -> const Component&;

  [[nodiscard]] auto find_property(StringView name) -> Attribute*;
  [[nodiscard]] auto find_property(StringView name) const -> const Attribute*;
  [[nodiscard]] auto find_component(const UUID& component_id) -> Component*;
  [[nodiscard]] auto find_component(const UUID& component_id) const -> const Component*;

  [[nodiscard]] auto has_property(StringView name) const -> bool;
  [[nodiscard]] auto has_component(const UUID& component_id) const -> bool;

  [[nodiscard]] auto property_count() const -> usize;
  [[nodiscard]] auto component_count() const -> usize;

  [[nodiscard]] auto clone() const -> ContextInfo;

  [[nodiscard]] auto get_uuid() const -> const UUID& { return mId; }

  [[nodiscard]] auto name() const -> const String& { return mName; }

 private:
  UUID mId {UUID::generate()};
  String mName;
  StringMap<Attribute> mProperties;
  HashMap<UUID, Component> mComponents;
};

}  // namespace tactile

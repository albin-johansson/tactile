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

#include "common/type/hash_map.hpp"
#include "common/type/result.hpp"
#include "common/type/string.hpp"
#include "common/type/string_map.hpp"
#include "common/type/uuid.hpp"
#include "core/attribute.hpp"
#include "core/component/component.hpp"
#include "tactile/base/container/function.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"

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

  auto add_property(String name, Attribute value) -> Result;

  auto add_property(String name, AttributeType type) -> Result;

  auto update_property(StringView name, Attribute value) -> Result;

  auto remove_property(StringView name) -> Maybe<Attribute>;

  auto rename_property(StringView current_name, String new_name) -> Result;

  /// Visits each component attached to the context.
  void each_component(const ComponentVisitor& visitor) const;

  /// Attaches a component to the context.
  /// Has no effect if the context already has an attached component of the same type.
  auto attach_component(Component component) -> Result;

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
  UUID mId {make_uuid()};
  String mName;
  StringMap<Attribute> mProperties;
  HashMap<UUID, Component> mComponents;
};

}  // namespace tactile

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

#include "common/type/fn.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/maybe.hpp"
#include "common/type/string.hpp"
#include "common/type/result.hpp"
#include "common/type/uuid.hpp"
#include "core/component/component.hpp"
#include "core/context/property_bundle.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

class ContextInfo final {
 public:
  using ComponentVisitor = Fn<void(const UUID&, const Component&)>;

  TACTILE_DEFAULT_COPY(ContextInfo);
  TACTILE_DEFAULT_MOVE(ContextInfo);

  ContextInfo() = default;

  explicit ContextInfo(const UUID& id);

  void set_name(String name);

  void each_component(const ComponentVisitor& visitor) const;

  /// Attaches a component to the context.
  /// Has no effect if the context already has an attached component of the same type.
  auto attach_component(Component component) -> Result;

  /// Detaches a component from the context.
  auto detach_component(const UUID& component_id) -> Maybe<Component>;

  [[nodiscard]] auto get_component(const UUID& component_id) -> Component&;
  [[nodiscard]] auto get_component(const UUID& component_id) const -> const Component&;

  [[nodiscard]] auto find_component(const UUID& component_id) -> Component*;
  [[nodiscard]] auto find_component(const UUID& component_id) const -> const Component*;

  [[nodiscard]] auto has_component(const UUID& component_id) const -> bool;

  [[nodiscard]] auto component_count() const -> usize;

  [[nodiscard]] auto clone() const -> ContextInfo;

  [[nodiscard]] auto get_uuid() const -> const UUID& { return mId; }

  [[nodiscard]] auto name() const -> const String& { return mName; }

  [[nodiscard]] auto props() -> PropertyBundle& { return mProps; }
  [[nodiscard]] auto props() const -> const PropertyBundle& { return mProps; }

 private:
  UUID mId {make_uuid()};
  String mName;
  PropertyBundle mProps;
  HashMap<UUID, Component> mComponents;
};

}  // namespace tactile

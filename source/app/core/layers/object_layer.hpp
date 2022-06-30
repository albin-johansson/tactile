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

#include <boost/uuid/uuid_hash.hpp>

#include "core/common/associative.hpp"
#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/layers/layer.hpp"
#include "core/layers/layer_delegate.hpp"
#include "core/layers/object.hpp"

namespace tactile::core {

class ObjectLayer final : public ILayer
{
 public:
  [[nodiscard]] static auto make() -> Shared<ObjectLayer>;

  void accept(ILayerVisitor& visitor) override;
  void accept(IConstLayerVisitor& visitor) const override;

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  void set_parent(const Maybe<UUID>& parentId) override;

  void accept(IContextVisitor& visitor) const override;

  void set_name(std::string name) override;

  void add_object(Object object);

  void remove_object(const UUID& id);

  void reserve_objects(usize n);

  void select_object(const Maybe<UUID>& id);

  [[nodiscard]] auto object_count() const -> usize;

  [[nodiscard]] auto active_object_id() const -> Maybe<UUID>;

  [[nodiscard]] auto get_object_ptr( const UUID& id) -> const Shared<Object>&;

  [[nodiscard]] auto get_object(const UUID& id) -> Object&;
  [[nodiscard]] auto get_object(const UUID& id) const -> const Object&;

  [[nodiscard]] auto object_at(const Vector2f& pos) const -> Maybe<UUID>;

  [[nodiscard]] auto get_opacity() const -> float override;

  [[nodiscard]] auto is_visible() const -> bool override;

  [[nodiscard]] auto clone() const -> Shared<ILayer> override;

  [[nodiscard]] auto get_name() const -> const std::string& override;

  [[nodiscard]] auto get_props() -> PropertyBundle& override;
  [[nodiscard]] auto get_props() const -> const PropertyBundle& override;

  [[nodiscard]] auto get_comps() -> ComponentBundle& override;
  [[nodiscard]] auto get_comps() const -> const ComponentBundle& override;

  [[nodiscard]] auto get_parent() const -> Maybe<UUID> override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_type() const -> LayerType override
  {
    return LayerType::ObjectLayer;
  }

  [[nodiscard]] auto begin() const noexcept { return mObjects.begin(); }
  [[nodiscard]] auto end() const noexcept { return mObjects.end(); }

 private:
  LayerDelegate                 mDelegate;
  HashMap<UUID, Shared<Object>> mObjects;
  Maybe<UUID>                   mActiveObject;
};

}  // namespace tactile::core

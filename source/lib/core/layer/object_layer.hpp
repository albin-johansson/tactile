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

#include <boost/uuid/uuid_hash.hpp>

#include "common/numeric.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/maybe.hpp"
#include "common/type/ptr.hpp"
#include "common/type/uuid.hpp"
#include "core/layer/abstract_layer.hpp"
#include "core/layer/layer.hpp"
#include "core/layer/object.hpp"

namespace tactile {

/// Represents a collection of map objects, such as rectangles and points.
class ObjectLayer final : public AbstractLayer {
 public:
  void accept(ContextVisitor& visitor) const override;
  void accept(LayerVisitor& visitor) override;
  void accept(ConstLayerVisitor& visitor) const override;

  void add_object(Shared<Object> object);

  void remove_object(const UUID& id);

  void reserve_objects(usize n);

  void select_object(const Maybe<UUID>& id);

  [[nodiscard]] auto has_object(const UUID& id) const -> bool;

  [[nodiscard]] auto object_count() const -> usize;

  [[nodiscard]] auto active_object_id() const -> Maybe<UUID>;

  [[nodiscard]] auto get_object_ptr(const UUID& id) -> const Shared<Object>&;

  [[nodiscard]] auto get_object(const UUID& id) -> Object&;
  [[nodiscard]] auto get_object(const UUID& id) const -> const Object&;

  [[nodiscard]] auto object_at(const Float2& pos, const Float2& tile_size) const
      -> Maybe<UUID>;

  [[nodiscard]] auto clone() const -> Shared<Layer> override;

  [[nodiscard]] auto get_type() const -> LayerType override
  {
    return LayerType::ObjectLayer;
  }

  [[nodiscard]] auto begin() const noexcept { return mObjects.begin(); }
  [[nodiscard]] auto end() const noexcept { return mObjects.end(); }

 private:
  HashMap<UUID, Shared<Object>> mObjects;
  Maybe<UUID> mActiveObject;
};

}  // namespace tactile

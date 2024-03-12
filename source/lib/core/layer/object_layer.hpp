// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <boost/uuid/uuid_hash.hpp>

#include "core/layer/abstract_layer.hpp"
#include "core/layer/layer.hpp"
#include "core/layer/object.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/util/uuid.hpp"

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

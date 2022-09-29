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
#include <vector>      // vector

#include "core/common/uuid.hpp"
#include "core/common/vocabulary.hpp"
#include "core/layer/layer.hpp"
#include "core/layer/layer_delegate.hpp"
#include "core/type/ptr.hpp"

namespace tactile {

class GroupLayer final : public Layer {
 public:
  using LayerStorage = std::vector<Shared<Layer>>;
  using SimpleVisitor = std::function<void(const Layer*)>;

  [[nodiscard]] static auto make() -> Shared<GroupLayer>;

  void accept(ContextVisitor& visitor) const override;
  void accept(LayerVisitor& visitor) override;
  void accept(ConstLayerVisitor& visitor) const override;

  /// Behaves the same as accept(), except for not including itself (the root)
  void each(LayerVisitor& visitor);
  void each(ConstLayerVisitor& visitor) const;

  void each(const SimpleVisitor& visitor) const;

  void add_layer(const UUID& parent, const Shared<Layer>& layer);
  void add_layer(Shared<Layer> layer);

  auto remove_layer(const UUID& id) -> Shared<Layer>;

  auto duplicate_layer(const UUID& id) -> Shared<Layer>;

  void move_layer_up(const UUID& id);
  void move_layer_down(const UUID& id);

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  void set_parent(const Maybe<UUID>& parent_id) override;

  void set_meta_id(int32 id) override;

  void set_layer_index(const UUID& id, usize index);

  [[nodiscard]] auto layer_count() const -> usize;

  [[nodiscard]] auto sibling_count(const UUID& id) const -> usize;

  [[nodiscard]] auto get_local_index(const UUID& id) const -> usize;

  [[nodiscard]] auto get_global_index(const UUID& id) const -> usize;

  [[nodiscard]] auto can_move_layer_up(const UUID& id) const -> bool;

  [[nodiscard]] auto can_move_layer_down(const UUID& id) const -> bool;

  [[nodiscard]] auto get_layer(const UUID& id) -> Shared<Layer>;

  [[nodiscard]] auto view_layer(const UUID& id) -> Layer&;
  [[nodiscard]] auto view_layer(const UUID& id) const -> const Layer&;

  [[nodiscard]] auto view_tile_layer(const UUID& id) -> TileLayer&;
  [[nodiscard]] auto view_tile_layer(const UUID& id) const -> const TileLayer&;

  [[nodiscard]] auto view_object_layer(const UUID& id) -> ObjectLayer&;
  [[nodiscard]] auto view_object_layer(const UUID& id) const -> const ObjectLayer&;

  [[nodiscard]] auto view_group_layer(const UUID& id) -> GroupLayer&;
  [[nodiscard]] auto view_group_layer(const UUID& id) const -> const GroupLayer&;

  [[nodiscard]] auto find_layer(const UUID& id) -> Layer*;
  [[nodiscard]] auto find_layer(const UUID& id) const -> const Layer*;

  [[nodiscard]] auto find_tile_layer(const UUID& id) -> TileLayer*;
  [[nodiscard]] auto find_tile_layer(const UUID& id) const -> const TileLayer*;

  [[nodiscard]] auto find_object_layer(const UUID& id) -> ObjectLayer*;
  [[nodiscard]] auto find_object_layer(const UUID& id) const -> const ObjectLayer*;

  [[nodiscard]] auto find_group_layer(const UUID& id) -> GroupLayer*;
  [[nodiscard]] auto find_group_layer(const UUID& id) const -> const GroupLayer*;

  [[nodiscard]] auto get_opacity() const -> float override;

  [[nodiscard]] auto is_visible() const -> bool override;

  [[nodiscard]] auto clone() const -> Shared<Layer> override;

  [[nodiscard]] auto ctx() -> ContextInfo& override;
  [[nodiscard]] auto ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_parent() const -> Maybe<UUID> override;

  [[nodiscard]] auto get_meta_id() const -> Maybe<int32> override;

  [[nodiscard]] auto storage() -> LayerStorage& { return mLayers; }
  [[nodiscard]] auto storage() const -> const LayerStorage& { return mLayers; }

  [[nodiscard]] auto get_type() const -> LayerType override
  {
    return LayerType::GroupLayer;
  }

 private:
  LayerDelegate mDelegate;
  LayerStorage mLayers;
};

}  // namespace tactile

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

#include "core/common/ints.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/fwd.hpp"
#include "core/layers/layer.hpp"
#include "core/layers/layer_delegate.hpp"

namespace tactile::core {

class GroupLayer final : public ILayer
{
 public:
  using LayerStorage = std::vector<Shared<ILayer>>;
  using SimpleVisitor = std::function<void(const ILayer*)>;

  [[nodiscard]] static auto make() -> Shared<GroupLayer>;

  void accept(ILayerVisitor& visitor) override;
  void accept(IConstLayerVisitor& visitor) const override;

  /// Behaves the same as accept(), except for not including itself (the root)
  void each(ILayerVisitor& visitor);
  void each(IConstLayerVisitor& visitor) const;

  void each(const SimpleVisitor& visitor) const;

  void add_layer(const UUID& parent, Shared<ILayer> layer);
  void add_layer(Shared<ILayer> layer);

  auto remove_layer(const UUID& id) -> Shared<ILayer>;

  void move_layer_up(const UUID& id);
  void move_layer_down(const UUID& id);

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  void set_parent(const Maybe<UUID>& parentId) override;

  void set_name(std::string name) override;

  [[nodiscard]] auto layer_count() const -> usize;

  [[nodiscard]] auto sibling_count(const UUID& id) const -> usize;

  [[nodiscard]] auto get_local_index(const UUID& id) const -> usize;

  [[nodiscard]] auto get_global_index(const UUID& id) const -> usize;

  [[nodiscard]] auto can_move_layer_up(const UUID& id) const -> bool;

  [[nodiscard]] auto can_move_layer_down(const UUID& id) const -> bool;

  [[nodiscard]] auto get_layer(const UUID& id) -> Shared<ILayer>;

  [[nodiscard]] auto view_layer(const UUID& id) -> ILayer&;
  [[nodiscard]] auto view_layer(const UUID& id) const -> const ILayer&;

  [[nodiscard]] auto view_tile_layer(const UUID& id) -> TileLayer&;
  [[nodiscard]] auto view_tile_layer(const UUID& id) const -> const TileLayer&;

  [[nodiscard]] auto view_object_layer(const UUID& id) -> ObjectLayer&;
  [[nodiscard]] auto view_object_layer(const UUID& id) const -> const ObjectLayer&;

  [[nodiscard]] auto view_group_layer(const UUID& id) -> GroupLayer&;
  [[nodiscard]] auto view_group_layer(const UUID& id) const -> const GroupLayer&;

  [[nodiscard]] auto find_layer(const UUID& id) -> ILayer*;
  [[nodiscard]] auto find_layer(const UUID& id) const -> const ILayer*;

  [[nodiscard]] auto find_tile_layer(const UUID& id) -> TileLayer*;
  [[nodiscard]] auto find_tile_layer(const UUID& id) const -> const TileLayer*;

  [[nodiscard]] auto find_object_layer(const UUID& id) -> ObjectLayer*;
  [[nodiscard]] auto find_object_layer(const UUID& id) const -> const ObjectLayer*;

  [[nodiscard]] auto find_group_layer(const UUID& id) -> GroupLayer*;
  [[nodiscard]] auto find_group_layer(const UUID& id) const -> const GroupLayer*;

  [[nodiscard]] auto get_opacity() const -> float override;

  [[nodiscard]] auto is_visible() const -> bool override;

  [[nodiscard]] auto clone() const -> Shared<ILayer> override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_name() const -> const std::string& override;

  [[nodiscard]] auto get_props() -> PropertyBundle& override;
  [[nodiscard]] auto get_props() const -> const PropertyBundle& override;

  [[nodiscard]] auto get_comps() -> ComponentBundle& override;
  [[nodiscard]] auto get_comps() const -> const ComponentBundle& override;

  [[nodiscard]] auto get_parent() const -> Maybe<UUID> override;

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

}  // namespace tactile::core

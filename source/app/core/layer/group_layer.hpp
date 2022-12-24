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

#include "core/layer/abstract_layer.hpp"
#include "core/type/fn.hpp"
#include "core/type/ptr.hpp"
#include "core/type/uuid.hpp"
#include "core/type/vec.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// A layer variant that serves as a container for other layers.
/// Group layers are recursive, and can store an unlimited amount of child layers.
class GroupLayer final : public AbstractLayer {
 public:
  using LayerStorage = Vec<Shared<Layer>>;
  using SimpleVisitor = Fn<void(const Layer*)>;

  void accept(ContextVisitor& visitor) const override;
  void accept(LayerVisitor& visitor) override;
  void accept(ConstLayerVisitor& visitor) const override;

  /// Behaves the same as accept(), except for not including itself (the root)
  void each(LayerVisitor& visitor);
  void each(ConstLayerVisitor& visitor) const;
  void each(const SimpleVisitor& visitor) const;

  /// Adds a layer to the hierarchy, with the specified group layer as the parent.
  void add(const UUID& parent, const Shared<Layer>& layer);

  /// Adds a layer as an immediate child of the group layer.
  void add(Shared<Layer> layer);

  /// Removes a child layer.
  auto remove(const UUID& id) -> Shared<Layer>;

  /// Duplicates an existing layer in the hierarchy.
  /// The new layer is inserted below the target layer (rendered above it).
  auto duplicate(const UUID& id) -> Shared<Layer>;

  /// Moves a layer up relative to its siblings (it will be rendered earlier).
  void move_up(const UUID& id);

  /// Moves a layer down relative to its siblings (it will be rendered later).
  void move_down(const UUID& id);

  /// Moves a layer to a specific index, relative to its siblings.
  void set_index(const UUID& id, usize index);

  /// Returns the total number of layers in the hierarchy.
  [[nodiscard]] auto size() const -> usize;

  /// Returns the amount of siblings for a specific layer.
  [[nodiscard]] auto sibling_count(const UUID& id) const -> usize;

  /// Returns the index of a layer in relation to its siblings.
  [[nodiscard]] auto local_index(const UUID& id) const -> usize;

  /// Returns the index of a layer when the hierarchy is iterated.
  [[nodiscard]] auto global_index(const UUID& id) const -> usize;

  /// Indicates whether a layer can be moved up.
  [[nodiscard]] auto can_move_up(const UUID& id) const -> bool;

  /// Indicates whether a layer can be moved down.
  [[nodiscard]] auto can_move_down(const UUID& id) const -> bool;

  [[nodiscard]] auto get_layer_ptr(const UUID& id) -> Shared<Layer>;

  [[nodiscard]] auto get_layer(const UUID& id) -> Layer&;
  [[nodiscard]] auto get_layer(const UUID& id) const -> const Layer&;

  [[nodiscard]] auto get_tile_layer(const UUID& id) -> TileLayer&;
  [[nodiscard]] auto get_tile_layer(const UUID& id) const -> const TileLayer&;

  [[nodiscard]] auto get_object_layer(const UUID& id) -> ObjectLayer&;
  [[nodiscard]] auto get_object_layer(const UUID& id) const -> const ObjectLayer&;

  [[nodiscard]] auto get_group_layer(const UUID& id) -> GroupLayer&;
  [[nodiscard]] auto get_group_layer(const UUID& id) const -> const GroupLayer&;

  [[nodiscard]] auto find_layer(const UUID& id) -> Layer*;
  [[nodiscard]] auto find_layer(const UUID& id) const -> const Layer*;

  [[nodiscard]] auto find_tile_layer(const UUID& id) -> TileLayer*;
  [[nodiscard]] auto find_tile_layer(const UUID& id) const -> const TileLayer*;

  [[nodiscard]] auto find_object_layer(const UUID& id) -> ObjectLayer*;
  [[nodiscard]] auto find_object_layer(const UUID& id) const -> const ObjectLayer*;

  [[nodiscard]] auto find_group_layer(const UUID& id) -> GroupLayer*;
  [[nodiscard]] auto find_group_layer(const UUID& id) const -> const GroupLayer*;

  [[nodiscard]] auto clone() const -> Shared<Layer> override;

  [[nodiscard]] auto storage() -> LayerStorage& { return mLayers; }
  [[nodiscard]] auto storage() const -> const LayerStorage& { return mLayers; }

  [[nodiscard]] auto get_type() const -> LayerType override
  {
    return LayerType::GroupLayer;
  }

 private:
  LayerStorage mLayers;
};

}  // namespace tactile

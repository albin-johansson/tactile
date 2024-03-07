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

#include "core/layer/abstract_layer.hpp"
#include "core/layer/visitors.hpp"
#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/function.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// A layer variant that serves as a container for other layers.
/// Group layers are recursive, and can store an unlimited amount of child layers.
class GroupLayer final : public AbstractLayer {
 public:
  using LayerStorage = Vector<Shared<Layer>>;
  using UnaryLayerFunc = Function<void(const Layer&)>;

  void accept(ContextVisitor& visitor) const override;
  void accept(LayerVisitor& visitor) override;
  void accept(ConstLayerVisitor& visitor) const override;

  /// Behaves the same as accept, except for not including itself (the root)
  void each(LayerVisitor& visitor);
  void each(ConstLayerVisitor& visitor) const;
  void each(const UnaryLayerFunc& func) const;

  /// Adds a layer as an immediate child of the group layer.
  ///
  /// \param layer the layer that will be added.
  /// \return nothing if the layer was added; an error code otherwise.
  auto add_layer(Shared<Layer> layer) -> Result<void>;

  /// Adds a layer to the hierarchy, with the specified group layer as the parent.
  ///
  /// \details
  /// This function does nothing if there is no group layer with the specified ID.
  ///
  /// \param parent_id the ID of a group layer to add the layer to.
  /// \param layer the layer that will be added, cannot be null.
  /// \return nothing if the layer was added; an error code otherwise.
  auto add_layer(const UUID& parent_id, const Shared<Layer>& layer) -> Result<void>;

  /// Attempts to remove a layer.
  ///
  /// \param layer_id the ID of the layer that will be removed.
  /// \return the removed layer, or null if no layer was removed.
  auto remove_layer(const UUID& layer_id) -> Shared<Layer>;

  /// Duplicates an existing layer in the hierarchy.
  ///
  /// \details
  /// The new layer is placed after the source layer in the hierarchy, so that it is
  /// visited after the source layer.
  ///
  /// \param layer_id the ID of the layer that will be duplicated.
  /// \return the new layer.
  auto duplicate_layer(const UUID& layer_id) -> Shared<Layer>;

  /// Moves a layer up relative to its siblings (it will be visited earlier).
  void move_layer_up(const UUID& layer_id);

  /// Moves a layer down relative to its siblings (it will be visited later).
  void move_layer_down(const UUID& layer_id);

  /// Attempts to move a layer so that its local index matches the specified index.
  ///
  /// \details
  /// There is no guarantee that the target layer will end up at the requested location.
  /// For example, if the index refers to an invalid location, the layer will be moved
  /// to the last valid location.
  ///
  /// \param layer_id the ID of the layer that will be moved.
  /// \param index the target local layer index.
  void set_layer_index(const UUID& layer_id, usize index);

  /// Returns the total number of layers in the hierarchy.
  [[nodiscard]] auto layer_count() const -> usize;

  /// Returns the amount of siblings for a specific layer.
  [[nodiscard]] auto layer_sibling_count(const UUID& layer_id) const -> usize;

  /// Returns the index of a layer in relation to its siblings.
  [[nodiscard]] auto local_layer_index(const UUID& layer_id) const -> usize;

  /// Returns the index of a layer when the hierarchy is iterated.
  [[nodiscard]] auto global_layer_index(const UUID& layer_id) const -> usize;

  /// Indicates whether a layer can be moved up.
  [[nodiscard]] auto can_move_layer_up(const UUID& layer_id) const -> bool;

  /// Indicates whether a layer can be moved down.
  [[nodiscard]] auto can_move_layer_down(const UUID& layer_id) const -> bool;

  [[nodiscard]] auto find_shared_layer(const UUID& layer_id) -> Shared<Layer>;

  [[nodiscard]] auto find_layer(const UUID& layer_id) -> Layer*;
  [[nodiscard]] auto find_layer(const UUID& layer_id) const -> const Layer*;

  [[nodiscard]] auto find_tile_layer(const UUID& layer_id) -> TileLayer*;
  [[nodiscard]] auto find_tile_layer(const UUID& layer_id) const -> const TileLayer*;

  [[nodiscard]] auto find_object_layer(const UUID& layer_id) -> ObjectLayer*;
  [[nodiscard]] auto find_object_layer(const UUID& layer_id) const -> const ObjectLayer*;

  [[nodiscard]] auto find_group_layer(const UUID& layer_id) -> GroupLayer*;
  [[nodiscard]] auto find_group_layer(const UUID& layer_id) const -> const GroupLayer*;

  [[nodiscard]] auto get_layer(const UUID& layer_id) -> Layer&;
  [[nodiscard]] auto get_layer(const UUID& layer_id) const -> const Layer&;

  [[nodiscard]] auto get_tile_layer(const UUID& layer_id) -> TileLayer&;
  [[nodiscard]] auto get_tile_layer(const UUID& layer_id) const -> const TileLayer&;

  [[nodiscard]] auto get_object_layer(const UUID& layer_id) -> ObjectLayer&;
  [[nodiscard]] auto get_object_layer(const UUID& layer_id) const -> const ObjectLayer&;

  [[nodiscard]] auto get_group_layer(const UUID& layer_id) -> GroupLayer&;
  [[nodiscard]] auto get_group_layer(const UUID& layer_id) const -> const GroupLayer&;

  [[nodiscard]] auto clone() const -> Shared<Layer> override;

  [[nodiscard]] auto get_storage() -> LayerStorage& { return mLayers; }
  [[nodiscard]] auto get_storage() const -> const LayerStorage& { return mLayers; }

  [[nodiscard]] auto get_type() const -> LayerType override
  {
    return LayerType::GroupLayer;
  }

 private:
  LayerStorage mLayers;

  void mutate_layer(const UUID& layer_id, const LayerMutatorFn& op);
  void query_layer(const UUID& layer_id, const LayerQueryFn& op) const;
};

}  // namespace tactile

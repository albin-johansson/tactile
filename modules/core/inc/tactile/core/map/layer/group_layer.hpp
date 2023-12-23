// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/map/layer/layer.hpp"
#include "tactile/core/map/layer/layer_behavior_delegate.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/function.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief A layer variant consisting of a collection of other layers.
 */
class TACTILE_CORE_API GroupLayer final : public ILayer {
 public:
  using LayerStorage = Vector<Shared<ILayer>>;
  using LayerCallback = Function<void(ILayer&)>;
  using ConstLayerCallback = Function<void(const ILayer&)>;

  void accept(IMetaContextVisitor& visitor) override;

  void accept(ILayerVisitor& visitor) override;

  void accept(IConstLayerVisitor& visitor) const override;

  /**
   * \brief Inspects each meta context in the group recursively.
   *
   * \note The callback is not called for the invoked group itself.
   *
   * \param visitor the visitor that will be called.
   */
  void each(IMetaContextVisitor& visitor) const;

  /**
   * \brief Inspects the layers in the group, but not the root group itself.
   *
   * \param visitor the layer visitor.
   */
  void each(ILayerVisitor& visitor);

  /** \copydoc each(ILayerVisitor&) */
  void each(IConstLayerVisitor& visitor) const;

  /**
   * \brief Invokes a callback for each layer in the group recursively.
   *
   * \note The callback is not called for the invoked group itself.
   *
   * \param callable a callback invoked for each stored layer.
   */
  void each(const LayerCallback& callable);

  /** \copydoc each(const LayerCallback&) */
  void each(const ConstLayerCallback& callable) const;

  /**
   * \brief Adds a layer to the group layer.
   *
   * \param layer the layer to add.
   */
  void append_layer(Shared<ILayer> layer);

  /**
   * \brief Adds a layer to a nested group layer.
   *
   * \param parent_uuid the UUID of the parent group layer.
   * \param layer       the layer to add.
   *
   * \return true if the layer was successfully added; false otherwise.
   */
  auto append_layer_to(const UUID& parent_uuid, Shared<ILayer> layer) -> bool;

  /**
   * \brief Removes a layer from the group layer.
   *
   * \param uuid the UUID associated with the layer to remove.
   *
   * \return the removed layer.
   */
  auto remove_layer(const UUID& uuid) -> Shared<ILayer>;

  /**
   * \brief Duplicates a layer in the hierarchy.
   *
   * \details The new layer is placed immediately after the source layer in the hierarchy,
   *          so that it would be visited after the source layer.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return the new layer.
   */
  auto duplicate_layer(const UUID& uuid) -> Shared<ILayer>;

  /**
   * \brief Shifts a layer one step up (earlier) relative to its siblings.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return true if a layer was moved; false otherwise.
   */
  auto move_layer_up(const UUID& uuid) -> bool;

  /**
   * \brief Shifts a layer one step down (later) relative to its siblings.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return true if a layer was moved; false otherwise.
   */
  auto move_layer_down(const UUID& uuid) -> bool;

  void set_persistent_id(Maybe<int32> id) override;

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  /**
   * \brief Indicates whether a layer can be moved up.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return true if the layer can be shifted up; false otherwise.
   */
  [[nodiscard]]
  auto can_move_layer_up(const UUID& uuid) const -> bool;

  /**
   * \brief Indicates whether a layer can be moved down.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return true if the layer can be shifted down; false otherwise.
   */
  [[nodiscard]]
  auto can_move_layer_down(const UUID& uuid) const -> bool;

  /**
   * \brief Returns the index of a layer in the group relative to its siblings.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return a local index.
   */
  [[nodiscard]]
  auto get_layer_local_index(const UUID& uuid) const -> Maybe<usize>;

  /**
   * \brief Returns the global (logical) index of a layer in the group.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return a global index.
   */
  [[nodiscard]]
  auto get_layer_global_index(const UUID& uuid) const -> Maybe<usize>;

  /**
   * \brief Attempts to find a nested layer.
   *
   * \note This function won't find the root group layer itself.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return a pointer to the found layer, or a null pointer.
   */
  [[nodiscard]]
  auto find_layer(const UUID& uuid) -> ILayer*;

  /** \copydoc find_layer() */
  [[nodiscard]]
  auto find_layer(const UUID& uuid) const -> const ILayer*;

  /**
   * \brief Attempts to find a nested tile layer.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return a pointer to the found layer, or a null pointer.
   */
  [[nodiscard]]
  auto find_tile_layer(const UUID& uuid) -> TileLayer*;

  /** \copydoc find_tile_layer() */
  [[nodiscard]]
  auto find_tile_layer(const UUID& uuid) const -> const TileLayer*;

  /**
   * \brief Attempts to find a nested object layer.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return a pointer to the found layer, or a null pointer.
   */
  [[nodiscard]]
  auto find_object_layer(const UUID& uuid) -> ObjectLayer*;

  /** \copydoc find_object_layer() */
  [[nodiscard]]
  auto find_object_layer(const UUID& uuid) const -> const ObjectLayer*;

  /**
   * \brief Attempts to find a nested group layer.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return a pointer to the found layer, or a null pointer.
   */
  [[nodiscard]]
  auto find_group_layer(const UUID& uuid) -> GroupLayer*;

  /** \copydoc find_group_layer() */
  [[nodiscard]]
  auto find_group_layer(const UUID& uuid) const -> const GroupLayer*;

  /**
   * \brief Attempts to find the group layer that hosts the specified layer.
   *
   * \param uuid the UUID associated with the target layer.
   *
   * \return a pointer to the found layer, or a null pointer.
   */
  [[nodiscard]]
  auto find_parent_layer(const UUID& uuid) -> GroupLayer*;

  /**
   * \brief Returns the total (recursive) number of sublayers stored in the group.
   *
   * \return a layer count.
   */
  [[nodiscard]]
  auto layer_count() const -> usize;

  [[nodiscard]]
  auto get_persistent_id() const -> Maybe<int32> override;

  [[nodiscard]]
  auto get_opacity() const -> float override;

  [[nodiscard]]
  auto is_visible() const -> bool override;

  [[nodiscard]]
  auto clone() const -> Shared<ILayer> override;

  [[nodiscard]]
  auto get_meta() -> Metadata& override;

  [[nodiscard]]
  auto get_meta() const -> const Metadata& override;

 private:
  LayerBehaviorDelegate mDelegate;
  LayerStorage mLayers;

  friend class ParentLayerFinder;
  friend class ConstParentLayerFinder;

  struct FindResult final {
    LayerStorage* storage;
    LayerStorage::iterator iter;
  };

  struct ConstFindResult final {
    const LayerStorage* storage;
    LayerStorage::const_iterator iter;
  };

  [[nodiscard]]
  auto _recursive_find(const UUID& uuid) -> Maybe<FindResult>;

  [[nodiscard]]
  auto _recursive_find(const UUID& uuid) const -> Maybe<ConstFindResult>;

  [[nodiscard]]
  static auto _can_move_layer_up(const LayerStorage& storage,
                                 LayerStorage::const_iterator iter) -> bool;

  [[nodiscard]]
  static auto _can_move_layer_down(const LayerStorage& storage,
                                   LayerStorage::const_iterator iter) -> bool;
};

}  // namespace tactile

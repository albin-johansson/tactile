// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/map/layer/layer.hpp"
#include "tactile/core/map/layer/layer_behavior_delegate.hpp"
#include "tactile/core/map/layer/object.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/container/tree_map.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief A layer variant consisting of an arbitrary collection of objects.
 */
class TACTILE_CORE_API ObjectLayer final : public ILayer {
 public:
  void accept(IMetaContextVisitor& visitor) override;

  void accept(ILayerVisitor& visitor) override;

  void accept(IConstLayerVisitor& visitor) const override;

  /**
   * \brief Adds an object to the layer.
   *
   * \param object the object to add.
   *
   * \return nothing on success; an error code otherwise.
   */
  auto add_object(Object object) -> Result<void>;

  /**
   * \brief Creates a object and adds it to the layer.
   *
   * \param type the type of the new object.
   *
   * \return the created object.
   */
  auto emplace_object(ObjectType type) -> Object&;

  /**
   * \brief Removes an object from the layer.
   *
   * \param uuid the target object UUID.
   *
   * \return the removed object.
   */
  auto remove_object(const UUID& uuid) -> Maybe<Object>;

  void set_persistent_id(Maybe<int32> id) override;

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  /**
   * \brief Returns an object in the layer.
   *
   * \param uuid the UUID of the desired object.
   *
   * \return the requested object.
   */
  [[nodiscard]]
  auto get_object(const UUID& uuid) -> Object&;

  /** \copydoc get_object() */
  [[nodiscard]]
  auto get_object(const UUID& uuid) const -> const Object&;

  /**
   * \brief Attempts to find and return an object in the layer.
   *
   * \param uuid the UUID of the desired object.
   *
   * \return a pointer to the found object, or a null pointer.
   */
  [[nodiscard]]
  auto find_object(const UUID& uuid) -> Object*;

  /** \copydoc find_object() */
  [[nodiscard]]
  auto find_object(const UUID& uuid) const -> const Object*;

  /**
   * \brief Indicates whether the layer contains an object with the specified UUID.
   *
   * \param uuid the object UUID to look for.
   *
   * \return true if an object was found; false otherwise.
   */
  [[nodiscard]]
  auto has_object(const UUID& uuid) const -> bool;

  /**
   * \brief Returns the number of objects in the layer.
   *
   * \return an object count.
   */
  [[nodiscard]]
  auto object_count() const -> ssize;

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
  TreeMap<UUID, Object> mObjects;  // TODO HashMap? Vector with UUIDs for ordering?
};

}  // namespace tactile

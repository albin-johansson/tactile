// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/map/layer/layer.hpp"
#include "tactile/core/map/layer/layer_behavior_delegate.hpp"
#include "tactile/core/map/layer/object.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/container/tree_map.hpp"
#include "tactile/foundation/container/vector.hpp"
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
   * \note An existing object in the layer with the specified ID will be overwritten.
   *
   * \param id     the ID of the object.
   * \param object the object to add.
   */
  void add_object(ObjectID id, Shared<Object> object);

  /**
   * \brief Removes an object from the layer.
   *
   * \param id the ID associated with the object.
   *
   * \return the removed object.
   */
  auto remove_object(ObjectID id) -> Shared<Object>;

  void set_persistent_id(Maybe<int32> id) override;

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  /**
   * \brief Returns an object in the layer.
   *
   * \note Avoid calling this function if you only need to "look" at the returned object,
   *       prefer calling `find_object` in such cases. This avoids expensive copies of
   *       shared pointers.
   *
   * \param id the ID of the desired object.
   *
   * \return a shared pointer to the found object.
   */
  [[nodiscard]]
  auto get_object(ObjectID id) -> Shared<Object>;

  /**
   * \brief Attempts to find and return an object in the layer.
   *
   * \param id the ID of the desired object.
   *
   * \return a pointer to the found object, or a null pointer.
   */
  [[nodiscard]]
  auto find_object(ObjectID id) -> Object*;

  /**
   * \copydoc find_object()
   */
  [[nodiscard]]
  auto find_object(ObjectID id) const -> const Object*;

  /**
   * \brief Indicates whether the layer contains an object with the specified ID.
   *
   * \param id the object ID to check.
   *
   * \return true if an object was found; false otherwise.
   */
  [[nodiscard]]
  auto has_object(ObjectID id) const -> bool;

  /**
   * \brief Returns the number of objects in the layer.
   *
   * \return an object count.
   */
  [[nodiscard]]
  auto object_count() const -> usize;

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
  TreeMap<ObjectID, Shared<Object>> mObjects;
};

}  // namespace tactile

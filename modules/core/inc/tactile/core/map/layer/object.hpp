// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/meta/meta_context.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/misc/object_type.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a single object in an object layer.
 *
 * \see `ObjectLayer`
 */
class TACTILE_CORE_API Object final : public IMetaContext {
 public:
  /**
   * \brief Creates a map object with position (0, 0) and size (0, 0).
   *
   * \param type the initial object type.
   */
  explicit Object(ObjectType type);

  void accept(IMetaContextVisitor& visitor) override;

  /**
   * \brief Sets the type of the object.
   *
   * \param type an object type.
   */
  void set_type(ObjectType type);

  /**
   * \brief Sets the position of the object.
   *
   * \param position the new position.
   */
  void set_position(Float2 position);

  /**
   * \brief Sets the size of the object.
   *
   * \param size the new size.
   */
  void set_size(Float2 size);

  /**
   * \brief Sets the associated type tag.
   *
   * \param tag an arbitrary string.
   */
  void set_tag(String tag);

  /**
   * \brief Sets the associated persistent ID.
   *
   * \details This is used to track objects in save files.
   *
   * \param id a save file identifier.
   */
  void set_persistent_id(Maybe<int32> id);

  /**
   * \brief Sets whether the object is visible.
   *
   * \param visible true to make the object visible; false otherwise.
   */
  void set_visible(bool visible);

  /**
   * \brief Returns the type of the object.
   *
   * \return an object type.
   */
  [[nodiscard]]
  auto get_type() const -> ObjectType;

  /**
   * \brief Returns the position of the object.
   *
   * \return a position.
   */
  [[nodiscard]]
  auto get_position() const -> Float2;

  /**
   * \brief Returns the size of the object.
   *
   * \return a size.
   */
  [[nodiscard]]
  auto get_size() const -> Float2;

  /**
   * \brief Returns the associated tag, if any.
   *
   * \return a potentially empty string.
   */
  [[nodiscard]]
  auto get_tag() const -> const String&;

  /**
   * \brief Returns the associated persistent ID.
   *
   * \return a save file identifier.
   */
  [[nodiscard]]
  auto get_persistent_id() const -> Maybe<int32>;

  /**
   * \brief Indicates whether the object is visible.
   *
   * \return true if the object is visible; false otherwise.
   */
  [[nodiscard]]
  auto is_visible() const -> bool;

  [[nodiscard]]
  auto meta() -> Metadata& override;

  [[nodiscard]]
  auto meta() const -> const Metadata& override;

  /**
   * \brief Creates a clone of the object.
   *
   * \note Any associated persistent identifier is not copied.
   *
   * \return an object.
   */
  [[nodiscard]]
  auto clone() const -> Object;

 private:
  ObjectType mType;
  Float2 mPosition {};
  Float2 mSize {};
  Metadata mMeta;
  String mTag;
  Maybe<int32> mPersistentId;
  bool mVisible {true};
};

}  // namespace tactile

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "core/layer/object_type.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/// Represents various map objects found in object layers.
/// The current supported object types are rectangles, ellipses, and points.
class Object final : public Context {
 public:
  TACTILE_DEFAULT_COPY(Object);
  TACTILE_DEFAULT_MOVE(Object);

  /// Creates an object of the specified type.
  explicit Object(ObjectType type = ObjectType::Rect);

  void accept(ContextVisitor& visitor) const override;

  /// Sets the object position.
  void set_pos(const Float2& pos);

  /// Sets the object size.
  /// May be called on point objects, the size is just not used for such objects.
  void set_size(const Float2& size);

  /// Changes the type of the object.
  /// When converting to point, both width and height are set to zero.
  void set_type(ObjectType type);

  /// Sets a tag associated with the object.
  /// This can be used to classify objects, e.g. "spawn-point" for a point object.
  void set_tag(String tag);

  /// Sets an associated "meta" identifier.
  /// This is used to track objects in save files, instead of UUIDs.
  void set_meta_id(int32 id);

  /// Sets whether the object is rendered.
  void set_visible(bool visible);

  [[nodiscard]] auto get_ctx() -> ContextInfo& override;
  [[nodiscard]] auto get_ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_type() const -> ObjectType;

  [[nodiscard]] auto is_rect() const -> bool;
  [[nodiscard]] auto is_ellipse() const -> bool;
  [[nodiscard]] auto is_point() const -> bool;

  [[nodiscard]] auto get_pos() const -> const Float2&;

  [[nodiscard]] auto get_size() const -> const Float2&;

  [[nodiscard]] auto get_tag() const -> const String&;

  /// Returns the associated meta identifier, if there is one.
  [[nodiscard]] auto get_meta_id() const -> Maybe<int32>;

  [[nodiscard]] auto is_visible() const -> bool;

  /// Creates a copy of the object, with a new UUID.
  [[nodiscard]] auto clone() const -> Shared<Object>;

 private:
  ContextInfo mContext;
  Float2 mPos {};            ///< Object position.
  Float2 mSize {};           ///< Object size (might be zero).
  ObjectType mType;          ///< Specific object type.
  String mTag;               ///< Optional user-provided tag.
  Maybe<int32> mMetaId;      ///< Identifier used in save files.
  bool mVisible : 1 {true};  ///< Is the object rendered?
};

}  // namespace tactile

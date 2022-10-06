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

#include "core/common/vocabulary.hpp"
#include "core/ctx/context.hpp"
#include "core/ctx/context_info.hpp"
#include "core/layer/object_type.hpp"
#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/type/ptr.hpp"
#include "core/type/string.hpp"

namespace tactile {

/// Represents various map objects found in object layers.
/// The current supported object types are rectangles, ellipses, and points.
class Object final : public Context {
 public:
  TACTILE_DEFAULT_COPY(Object);
  TACTILE_DEFAULT_MOVE(Object);

  explicit Object(ObjectType type = ObjectType::Rect);

  void accept(ContextVisitor& visitor) const override;

  void set_pos(const Float2& pos);
  void set_size(const Float2& size);

  /// Changes the type of the object.
  /// When converting to point, both width and height are set to zero.
  void set_type(ObjectType type);

  void set_tag(String tag);

  void set_meta_id(int32 id);

  void set_visible(bool visible);

  [[nodiscard]] auto ctx() -> ContextInfo& override;
  [[nodiscard]] auto ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_type() const -> ObjectType;

  [[nodiscard]] auto is_rect() const -> bool;
  [[nodiscard]] auto is_ellipse() const -> bool;
  [[nodiscard]] auto is_point() const -> bool;

  [[nodiscard]] auto get_pos() const -> const Float2&;
  [[nodiscard]] auto get_size() const -> const Float2&;

  [[nodiscard]] auto get_tag() const -> const String&;

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

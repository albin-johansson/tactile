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

#include <string>  // string

#include "core/common/math.hpp"
#include "core/contexts/context.hpp"
#include "core/contexts/context_delegate.hpp"
#include "core/layers/object_type.hpp"

namespace tactile::core {

class Object final : public IContext
{
 public:
  void set_pos(const Vector2f& pos);

  void set_size(const Vector2f& size);

  void set_type(ObjectType type);

  void set_tag(std::string tag);

  void set_visible(bool visible);

  void set_name(std::string name) override;

  void accept(IContextVisitor& visitor) const override;

  [[nodiscard]] auto get_props() -> PropertyBundle& override;
  [[nodiscard]] auto get_props() const -> const PropertyBundle& override;

  [[nodiscard]] auto get_comps() -> ComponentBundle& override;
  [[nodiscard]] auto get_comps() const -> const ComponentBundle& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_name() const -> const std::string& override;

  [[nodiscard]] auto get_type() const noexcept -> ObjectType { return mType; }

  [[nodiscard]] auto get_pos() const noexcept -> const Vector2f& { return mPos; }

  [[nodiscard]] auto get_size() const noexcept -> const Vector2f& { return mSize; }

  [[nodiscard]] auto get_tag() const -> const std::string& { return mTag; }

  [[nodiscard]] auto is_visible() const noexcept -> bool { return mVisible; }

 private:
  ContextDelegate mDelegate;
  Vector2f        mPos{};                   /// Object position.
  Vector2f        mSize{};                  /// Object size (might be zero).
  ObjectType      mType{ObjectType::Rect};  /// Specific object type.
  std::string     mTag;                     /// Optional user-provided tag.
  bool            mVisible : 1 {true};
};

}  // namespace tactile::core

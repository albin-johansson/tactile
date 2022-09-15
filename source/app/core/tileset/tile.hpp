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

#include <boost/uuid/uuid_hash.hpp>

#include "core/common/associative.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/math.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/ctx/context.hpp"
#include "core/ctx/context_info.hpp"
#include "core/layer/object.hpp"
#include "core/tileset/tile_animation.hpp"

namespace tactile {

/// Provides metadata for tiles in a tileset.
class Tile final : public IContext {
 public:
  explicit Tile(TileIndex index);

  void accept(IContextVisitor& visitor) const override;

  void update();

  void reserve_objects(usize n);

  void add_object(Shared<Object> object);

  void clear_animation();

  void set_animation(TileAnimation animation);

  void set_source(const int4& source);

  [[nodiscard]] auto object_count() const -> usize;

  [[nodiscard]] auto object_capacity() const -> usize;

  [[nodiscard]] auto get_objects() const -> const HashMap<UUID, Shared<Object>>&;

  [[nodiscard]] auto is_animated() const -> bool;

  [[nodiscard]] auto get_animation() const -> const TileAnimation&;

  [[nodiscard]] auto ctx() -> ContextInfo& override;
  [[nodiscard]] auto ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto index() const noexcept -> TileIndex { return mIndex; }

  [[nodiscard]] auto source() const noexcept -> const int4& { return mSource; }

 private:
  TileIndex mIndex;
  ContextInfo mContext;
  int4 mSource;
  Maybe<TileAnimation> mAnimation;
  HashMap<UUID, Shared<Object>> mObjects;
};

}  // namespace tactile

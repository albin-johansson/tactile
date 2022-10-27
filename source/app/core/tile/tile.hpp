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

#include "core/ctx/context.hpp"
#include "core/ctx/context_info.hpp"
#include "core/layer/object.hpp"
#include "core/tile/tile_animation.hpp"
#include "core/type/hash_map.hpp"
#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/type/ptr.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// Provides metadata for a tile in a tileset.
class Tile final : public Context {
 public:
  using ObjectMap = HashMap<UUID, Shared<Object>>;

  /// Creates a tile with a specific index.
  explicit Tile(TileIndex index);

  void accept(ContextVisitor& visitor) const override;

  /// Updates the state of the tile animation, if there is one.
  void update();

  /// Reserves capacity for a certain amount of objects (before reallocation).
  void reserve_objects(usize n);

  /// Adds an object to the tile.
  void add_object(Shared<Object> object);

  /// Removes any associated animation.
  void clear_animation();

  /// Adds an animation to the tile.
  void set_animation(TileAnimation animation);

  /// Sets the tile region in the associated tile set.
  void set_source(const Int4& source);

  /// Returns the amount of embedded objects.
  [[nodiscard]] auto object_count() const -> usize;

  /// Returns all embedded objects.
  [[nodiscard]] auto get_objects() const -> const ObjectMap&;

  /// Indicates whether the tile has an animation.
  [[nodiscard]] auto is_animated() const -> bool;

  /// Returns the associated animation, if there is one.
  [[nodiscard]] auto animation() const -> const TileAnimation&;

  [[nodiscard]] auto ctx() -> ContextInfo& override;
  [[nodiscard]] auto ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto uuid() const -> const UUID& override;

  /// Returns the index of the tile in the associated tile set.
  [[nodiscard]] auto index() const noexcept -> TileIndex { return mIndex; }

  /// Returns the tile region in the associated tile set.
  [[nodiscard]] auto source() const noexcept -> const Int4& { return mSource; }

 private:
  TileIndex mIndex;
  ContextInfo mContext;
  Int4 mSource {};
  Maybe<TileAnimation> mAnimation;
  ObjectMap mObjects;
};

}  // namespace tactile
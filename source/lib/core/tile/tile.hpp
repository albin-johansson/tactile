// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <boost/uuid/uuid_hash.hpp>

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "core/layer/object.hpp"
#include "core/tile/tile_animation.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

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
  [[nodiscard]] auto get_animation() -> TileAnimation&;
  [[nodiscard]] auto get_animation() const -> const TileAnimation&;

  /// Returns the index of the tile in the associated tile set.
  [[nodiscard]] auto get_index() const noexcept -> TileIndex { return mIndex; }

  /// Returns the tile region in the associated tile set.
  [[nodiscard]] auto get_source() const noexcept -> const Int4& { return mSource; }

  [[nodiscard]] auto get_ctx() -> ContextInfo& override;
  [[nodiscard]] auto get_ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

 private:
  TileIndex mIndex;
  ContextInfo mContext;
  Int4 mSource {};
  Maybe<TileAnimation> mAnimation;
  ObjectMap mObjects;
};

}  // namespace tactile

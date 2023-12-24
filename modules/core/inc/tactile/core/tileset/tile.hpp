// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/context/meta_context.hpp"
#include "tactile/core/context/metadata.hpp"
#include "tactile/core/map/layer/object.hpp"
#include "tactile/core/tileset/tile_animation.hpp"
#include "tactile/foundation/container/hash_map.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/math/rectangle.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Describes a tile in a tileset.
 */
class TACTILE_CORE_API Tile final : public IMetaContext {
 public:
  /**
   * \brief Creates a tile definition.
   *
   * \param region a region that encapsulates the tile in the associated tileset.
   */
  explicit Tile(const Rectangle& region);

  void accept(IMetaContextVisitor& visitor) override;

  /**
   * \brief Sets the associated animation.
   *
   * \param animation an animation, or nothing to remove the animation.
   */
  void set_animation(Maybe<TileAnimation> animation);

  [[nodiscard]]
  auto meta() -> Metadata& override;

  [[nodiscard]]
  auto meta() const -> const Metadata& override;

  /**
   * \brief Returns the tile region in the associated tileset.
   *
   * \return the tile region.
   */
  [[nodiscard]]
  auto get_region() const -> const Rectangle&;

  /**
   * \brief Returns the associated animation.
   *
   * \pre The tile must be animated.
   *
   * \return a tile animation.
   */
  [[nodiscard]]
  auto get_animation() -> TileAnimation&;

  /**
   * \copydoc get_animation()
   */
  [[nodiscard]]
  auto get_animation() const -> const TileAnimation&;

  /**
   * \brief Indicates whether the tile is animated.
   *
   * \return true if there's an associated animation; false otherwise.
   */
  [[nodiscard]]
  auto is_animated() const -> bool;

 private:
  Metadata mMeta;
  Rectangle mRegion;
  Maybe<TileAnimation> mAnimation;
  HashMap<UUID, Shared<Object>> mObjects;
};

}  // namespace tactile

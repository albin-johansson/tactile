// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Provides limitations for document viewports.
 */
struct DocumentViewportLimits final {
  Float2 min_offset {};  ///< The smallest possible offset (for both axes).
  Float2 max_offset {};  ///< The largest possible offset (for both axes).
};

/**
 * Represents the viewport associated with a document.
 */
class TACTILE_CORE_API DocumentViewport final {
 public:
  /**
   * Updates the viewport limits.
   *
   * \param limits The new limits, if any.
   */
  void reset_limits(const Maybe<DocumentViewportLimits>& limits = kNothing);

  /**
   * Nudges the viewport translation.
   *
   * \param delta The offset delta.
   */
  void translate(Float2 delta);

  /**
   * Updates the associated tile size used during calculations.
   *
   * \param tile_size The new tile size.
   */
  void set_tile_size(Float2 tile_size);

  /**
   * Returns the associated viewport origin offset.
   *
   * \return
   *    An offset.
   */
  [[nodiscard]]
  auto translation() const -> const Float2&;

  /**
   * Returns the associated viewport tile size.
   *
   * \return
   *    A tile size.
   */
  [[nodiscard]]
  auto tile_size() const -> const Float2&;

  /**
   * Returns the associated limits, if any.
   *
   * \return
   *    A possibly null pointer to the associated limits.
   */
  [[nodiscard]]
  auto limits() const -> const DocumentViewportLimits*;

 private:
  Float2 mTranslation {0, 0};
  Float2 mTileSize {32, 32};
  Maybe<DocumentViewportLimits> mLimits {};
};

}  // namespace tactile

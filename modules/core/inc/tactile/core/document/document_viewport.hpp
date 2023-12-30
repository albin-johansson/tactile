// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Provides limitations for document viewports.
 */
struct DocumentViewportLimits final {
  Float2 min_offset {};  ///< The smallest possible offset (for both axes).
  Float2 max_offset {};  ///< The largest possible offset (for both axes).
};

/**
 * \brief Represents the viewport associated with a document.
 */
class TACTILE_CORE_API DocumentViewport final {
 public:
  /**
   * \brief Updates the viewport limits.
   *
   * \param limits the new limits, if any.
   */
  void reset_limits(const Maybe<DocumentViewportLimits>& limits = kNothing);

  /**
   * \brief Nudges the viewport translation.
   *
   * \param delta the offset delta.
   */
  void translate(Float2 delta);

  /**
   * \brief Updates the associated tile size used during calculations.
   *
   * \param tile_size the new tile size.
   */
  void set_tile_size(Float2 tile_size);

  /**
   * \brief Returns the associated viewport origin offset.
   *
   * \return an offset.
   */
  [[nodiscard]]
  auto translation() const -> const Float2&;

  /**
   * \brief Returns the associated viewport tile size.
   *
   * \return a tile size.
   */
  [[nodiscard]]
  auto tile_size() const -> const Float2&;

  /**
   * \brief Returns the associated limits, if any.
   *
   * \return a possibly null pointer to the associated limits.
   */
  [[nodiscard]]
  auto limits() const -> const DocumentViewportLimits*;

 private:
  Float2 mTranslation {0, 0};
  Float2 mTileSize {32, 32};
  Maybe<DocumentViewportLimits> mLimits {};
};

}  // namespace tactile

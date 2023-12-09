// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/misc/chrono.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a single frame in a tile animation.
 */
struct TileAnimationFrame final {
  TileIndex tile_index {};   ///< The index of the shown tile.
  Milliseconds duration {};  ///< The duration that the tile is shown.

  [[nodiscard]] auto operator==(const TileAnimationFrame&) const -> bool = default;
};

/**
 * \brief Represents an animation for a single tile.
 */
class TACTILE_CORE_API TileAnimation final {
 public:
  /**
   * \brief Updates the state of the animation.
   *
   * \details This function should be called every frame, to advance the animation.
   */
  void update();

  /**
   * \brief Adds a frame to the end of the animation.
   *
   * \param frame the frame to add.
   */
  void append_frame(const TileAnimationFrame& frame);

  /**
   * \brief Adds a frame to the animation at the specified position.
   *
   * \note This function does nothing if the index exceeds the number of frames.
   *
   * \param index the desired frame index.
   * \param frame the frame to add.
   */
  void insert_frame(usize index, const TileAnimationFrame& frame);

  /**
   * \brief Removes the frame at the specified index from the animation.
   *
   * \note This function does nothing if the index does not reference a valid frame.
   *
   * \param index the frame index.
   */
  void remove_frame(usize index);

  /**
   * \brief Returns the index of the currently shown frame.
   *
   * \return a frame index.
   */
  [[nodiscard]]
  auto get_current_frame_index() const -> usize;

  /**
   * \brief Returns the animation frame at the specified index.
   *
   * \param index the frame index.
   *
   * \return an animation frame.
   */
  [[nodiscard]]
  auto get_frame(usize index) -> TileAnimationFrame&;

  /**
   * \copydoc get_frame()
   */
  [[nodiscard]]
  auto get_frame(usize index) const -> const TileAnimationFrame&;

  /**
   * \brief Returns the number of frames in the animation.
   *
   * \return the animation frame amount.
   */
  [[nodiscard]]
  auto frame_count() const -> usize;

  [[nodiscard]] auto begin() const { return mFrames.begin(); }
  [[nodiscard]] auto end() const { return mFrames.end(); }

 private:
  Vector<TileAnimationFrame> mFrames;
  usize mCurrentFrameIndex {};
  SystemClockInstant mLastUpdate {};
};

}  // namespace tactile

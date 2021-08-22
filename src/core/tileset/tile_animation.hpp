#pragma once

#include <centurion.hpp>  // milliseconds
#include <vector>         // vector

#include "aliases/ints.hpp"
#include "aliases/tile_id.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

/**
 * \brief Represents a single frame during a tile animation.
 */
struct Frame final
{
  tile_id tile;                          ///< The associated tile.
  cen::milliseconds<uint32> duration{};  ///< The duration of the frame.
};

/**
 * \brief Represents a tile animation.
 */
class TileAnimation final
{
 public:
  /**
   * \brief Updates the state of the animation.
   */
  void Update();

  /**
   * \brief Adds a single frame to the end of the animation.
   *
   * \param frame the frame that will be added.
   */
  void AddFrame(Frame frame);

  /**
   * \brief Returns the current tile.
   *
   * \return the tile associated with the current frame.
   */
  [[nodiscard]] auto GetCurrentTile() const -> tile_id;

  /**
   * \brief Returns the current duration.
   *
   * \return the duration associated with the current frame.
   */
  [[nodiscard]] auto GetCurrentDuration() const -> cen::milliseconds<uint32>;

  /**
   * \brief Returns the animation frames.
   *
   * \return all associated frames.
   */
  [[nodiscard]] auto GetFrames() const -> const std::vector<Frame>&;

 private:
  usize mIndex{};
  cen::milliseconds<uint32> mLastUpdateTime{};
  std::vector<Frame> mFrames;
};

/// \} End of group core

}  // namespace Tactile

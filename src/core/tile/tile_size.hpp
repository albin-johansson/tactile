#pragma once

namespace tactile::core {

/**
 * \class TileSize
 *
 * \brief Represents the size of the visual representation of tiles from
 * tilesets.
 *
 * \since 0.1.0
 *
 * \headerfile tile_size.hpp
 */
class [[deprecated]] TileSize final
{
 public:
  /**
   * \brief Resets the tile size to the original size.
   *
   * \since 0.1.0
   */
  void Reset() noexcept;

  /**
   * \brief Increases the tile size by 10 percent.
   *
   * \since 0.1.0
   */
  void Increase();

  /**
   * \brief Decreases the tile size by 10 percent. Note, the tile size will
   * always be at least `MinimumSize()`.
   *
   * \since 0.1.0
   */
  void Decrease() noexcept;

  /**
   * \brief Sets the tile size, the tile size will always be at least
   * `MinimumSize()`.
   *
   * \param size the new tile size.
   *
   * \since 0.1.0
   */
  void SetSize(int size) noexcept;

  /**
   * \brief Returns the current tile size.
   *
   * \return the current tile size, will always be at least `MinimumSize()`.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto Get() const noexcept -> int
  {
    return mSize;
  }

  /**
   * \brief Returns the default size of tiles.
   *
   * \return the default tile size.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr static auto DefaultSize() noexcept -> int
  {
    return 100;
  }

  /**
   * \brief Returns the minimum size of tiles.
   *
   * \return the minimum tile size.
   *
   * \since 0.1.0
   */
  [[nodiscard]] constexpr static auto MinimumSize() noexcept -> int
  {
    return 5;
  }

 private:
  int mSize{DefaultSize()};
};

}  // namespace tactile::core

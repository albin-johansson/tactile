#pragma once

namespace tactile::core {

/**
 * @class tile_size
 *
 * @brief Represents the size of the visual representation of tiles from
 * tilesets.
 *
 * @since 0.1.0
 *
 * @headerfile tile_size.hpp
 */
class tile_size final
{
 public:
  /**
   * @brief Resets the tile size to the original size.
   *
   * @since 0.1.0
   */
  void reset() noexcept;

  /**
   * @brief Increases the tile size by 10 percent.
   *
   * @since 0.1.0
   */
  void increase();

  /**
   * @brief Decreases the tile size by 10 percent. Note, the tile size will
   * always be at least `min_size()`.
   *
   * @since 0.1.0
   */
  void decrease() noexcept;

  /**
   * @brief Sets the tile size, the tile size will always be at least
   * `min_size()`.
   *
   * @param size the new tile size.
   *
   * @since 0.1.0
   */
  void set_size(int size) noexcept;

  /**
   * @brief Returns the current tile size.
   *
   * @return the current tile size, will always be at least `min_size()`.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get() const noexcept -> int
  {
    return m_size;
  }

  /**
   * @brief Returns the default size of tiles.
   *
   * @return the default tile size.
   *
   * @since 0.1.0
   */
  [[nodiscard]] static constexpr auto default_size() noexcept -> int
  {
    return 50;
  }

  /**
   * @brief Returns the minimum size of tiles.
   *
   * @return the minimum tile size.
   *
   * @since 0.1.0
   */
  [[nodiscard]] static constexpr auto min_size() noexcept -> int
  {
    return 5;
  }

 private:
  int m_size{default_size()};
};

}  // namespace tactile::core

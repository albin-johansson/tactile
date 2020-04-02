#pragma once

namespace tactile {

/**
 * The <code>TileSize</code> class represents the size of the visual
 * representation of tiles from tile sheets.
 *
 * @since 0.1.0
 */
class TileSize final {
 public:
  TileSize() noexcept;

  /**
   * The default size of the tiles.
   *
   * @since 0.1.0
   */
  inline static constexpr int defaultSize = 50;

  /**
   * The minimum size of the tiles.
   *
   * @since 0.1.0
   */
  inline static constexpr int minimumSize = 5;

  TileSize(const TileSize&) = delete;

  TileSize(TileSize&&) = delete;

  ~TileSize() noexcept = default;

  /**
   * Resets the tile size to the original size.
   *
   * @since 0.1.0
   */
  void reset() noexcept;

  /**
   * Increases the tile size by 10 percent.
   *
   * @since 0.1.0
   */
  void increase() noexcept;

  /**
   * Decreases the tile size by 10 percent. Note, the tile size will always be
   * at least <code>minimumSize</code>.
   *
   * @since 0.1.0
   */
  void decrease() noexcept;

  /**
   * Sets the tile size, the tile size will always be at least
   * <code>minimumSize</code>.
   *
   * @param size the new tile size, will be clamped to be at least
   * <code>minimumSize</code>.
   * @since 0.1.0
   */
  void set_size(int size) noexcept;

  /**
   * Returns the current tile size.
   *
   * @return the current tile size, will always be at least the
   * <code>minimumSize</code>.
   * @since 0.1.0
   */
  [[nodiscard]] int size() const noexcept { return m_size; }

 private:
  int m_size;
};

}  // namespace tactile

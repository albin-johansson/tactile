#pragma once

class QPainter;

namespace tactile {

class TileMap;

/**
 * The <code>TileMapRenderer</code> class represents a renderer for rendering
 * tile maps. This class is a friend class of <code>TileMap</code> and
 * <code>TileLayer</code>.
 *
 * @see TileMap
 * @see TileLayer
 * @since 0.1.0
 */
class TileMapRenderer final {
 public:
  TileMapRenderer() noexcept = default;

  ~TileMapRenderer() noexcept = default;

  TileMapRenderer(const TileMapRenderer&) = delete;
  TileMapRenderer(TileMapRenderer&&) = delete;

  TileMapRenderer& operator=(const TileMapRenderer&) = delete;
  TileMapRenderer& operator=(TileMapRenderer&&) = delete;

  /**
   * Renders a tile map.
   *
   * @param painter the painter that will be used.
   * @param map the tile map that will be rendered.
   * @since 0.1.0
   */
  void render(QPainter& painter, const TileMap& map) const noexcept;
};

}  // namespace tactile

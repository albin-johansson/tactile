#pragma once

class QPainter;

namespace tactile {

class Tilemap;

/**
 * The <code>TilemapRenderer</code> class represents a renderer for rendering
 * tilemaps. This class is a friend class of <code>Tilemap</code> and
 * <code>TileLayer</code>.
 *
 * @see Tilemap
 * @see TileLayer
 * @since 0.1.0
 */
class TilemapRenderer final {
 public:
  TilemapRenderer() noexcept = default;

  ~TilemapRenderer() noexcept = default;

  TilemapRenderer(const TilemapRenderer&) = delete;
  TilemapRenderer(TilemapRenderer&&) = delete;

  TilemapRenderer& operator=(const TilemapRenderer&) = delete;
  TilemapRenderer& operator=(TilemapRenderer&&) = delete;

  /**
   * Renders a tilemap.
   *
   * @param painter the painter that will be used.
   * @param map the tilemap that will be rendered.
   * @since 0.1.0
   */
  void render(QPainter& painter, const Tilemap& map) const noexcept;
};

}  // namespace tactile

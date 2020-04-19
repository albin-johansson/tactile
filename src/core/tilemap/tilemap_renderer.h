#pragma once

#include <type_utils.h>

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

  /**
   * Renders a tilemap.
   *
   * @param painter the painter that will be used.
   * @param map the tilemap that will be rendered.
   * @since 0.1.0
   */
  void render(QPainter& painter, const Tilemap& map) const noexcept;
};

static_assert(validate_nothrow<TilemapRenderer>());

}  // namespace tactile

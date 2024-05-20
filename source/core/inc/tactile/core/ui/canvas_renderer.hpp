// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/matrix_extent.hpp"
#include "tactile/core/util/matrix_index.hpp"

namespace tactile {

struct CViewport;
class Color;
class Registry;

namespace ui {

class Window;

/// \addtogroup UI
/// \{

/**
 * Provides the canvas rendering API.
 *
 * \note
 * Instances of this class are lightweight, and are intended to be constructed
 * just before use and subsequently discarded. In other words, there should be
 * no caching of renderer instances.
 */
class CanvasRenderer final
{
 public:
  /**
   * Represents a region of tiles that should be rendered.
   */
  struct VisibleTileRegion final
  {
    /** The index of the top-left tile (inclusive). */
    MatrixIndex begin;

    /** The index of the bottom-right tile (exclusive). */
    MatrixIndex end;
  };

  struct VisibleRegion final
  {
    Float2 begin;
    Float2 end;
  };

  struct RenderBounds final
  {
    MatrixIndex begin;
    MatrixIndex end;
  };

  TACTILE_DELETE_COPY(CanvasRenderer);
  TACTILE_DELETE_MOVE(CanvasRenderer);

  /**
   * Creates a renderer.
   *
   * \param window    The associated window.
   * \param tile_size The logical tile size.
   * \param extent    The current extent of the content.
   * \param viewport  The state of the viewport.
   */
  CanvasRenderer(const Window& window,
                 const MatrixExtent& extent,
                 const Int2& tile_size,
                 const CViewport& viewport);

  ~CanvasRenderer() noexcept;

  /**
   * Renders a rectangle.
   *
   * \param screen_pos The rectangle position, in screen coordinates.
   * \param size       The rectangle size.
   * \param color      The line color.
   * \param thickness  The line thickness.
   */
  static void draw_rect(const Float2& screen_pos,
                        const Float2& size,
                        const Color& color,
                        float thickness = 1.0f);

  /**
   * Renders a filled rectangle.
   *
   * \param screen_pos The rectangle position, in screen coordinates.
   * \param size       The rectangle size.
   * \param color      The fill color.
   */
  static void fill_rect(const Float2& screen_pos,
                        const Float2& size,
                        const Color& color);

  /**
   * Clears the canvas with a given color.
   *
   * \param color The color to use.
   */
  void clear_canvas(const Color& color) const;

  /**
   * Renders an orthogonal grid over the canvas.
   *
   * \param color The line color.
   */
  void draw_orthogonal_grid(const Color& color) const;

  /**
   * Renders an outline around the tile at the specified position.
   *
   * \param world_pos The position of the tile, using world coordinates.
   * \param color     The outline color.
   */
  void draw_tile_outline(const Float2& world_pos, const Color& color) const;
  /**
   * Returns the region of visible tiles in the canvas.
   *
   * \return
   * A tile region.
   */
  [[nodiscard]]
  auto get_visible_tiles() const noexcept -> const VisibleTileRegion&;

  [[nodiscard]]
  auto get_visible_region() const -> const VisibleRegion&;

  [[nodiscard]]
  auto get_render_bounds() const -> const RenderBounds&;

  /**
   * Returns the rendered size of tiles in the canvas.
   *
   * \return
   * A tile size.
   */
  [[nodiscard]]
  auto get_canvas_tile_size() const -> Float2;

  [[nodiscard]]
  auto to_world_pos(const Float2& screen_pos) const noexcept -> Float2;

  [[nodiscard]]
  auto to_screen_pos(const Float2& world_pos) const noexcept -> Float2;

 private:
  /** The content scale factor used by the viewport. */
  float mCanvasScale;

  /** The content extent. */
  MatrixExtent mExtent;

  /** The viewport position relative to the content origin (world coords). */
  Float2 mViewportPos;

  /** The size of rendered tiles. */
  Float2 mCanvasTileSize;

  /** The top-left window corner position (absolute coords). */
  Float2 mWindowTL;

  /** The bottom-right window corner position (absolute coords). */
  Float2 mWindowBR;

  /** The region visible via the current viewport. */
  VisibleRegion mVisibleRegion;

  /** The tile region visible via the current viewport. */
  VisibleTileRegion mVisibleTiles;

  /** The content tile positions that are within the visible region. */
  RenderBounds mRenderBounds;
};

/// \}

}  // namespace ui
}  // namespace tactile

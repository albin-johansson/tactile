// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"
#include "tactile/base/util/matrix_index.hpp"
#include "tactile/core/entity/entity.hpp"

struct ImDrawList;

namespace tactile {

struct CViewport;
struct UColor;
class Registry;

namespace ui {

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
   * Represents a region of tiles that are visible (but may not be valid).
   */
  struct VisibleTileRegion final
  {
    /** The index of the top-left tile (inclusive). */
    MatrixIndex begin;

    /** The index of the bottom-right tile (exclusive). */
    MatrixIndex end;
  };

  /**
   * Represents a region that is visible in the viewport.
   */
  struct VisibleRegion final
  {
    /** The top-left position of the visible region. */
    Float2 begin;

    /** The bottom-right position of the visible region. */
    Float2 end;
  };

  /**
   * Represents a region of tiles that should be rendered.
   */
  struct RenderBounds final
  {
    /** The top-left tile index to render (inclusive). */
    MatrixIndex begin;

    /** The bottom-right tile index to render (exclusive). */
    MatrixIndex end;
  };

  TACTILE_DELETE_COPY(CanvasRenderer);
  TACTILE_DELETE_MOVE(CanvasRenderer);

  /**
   * Creates a renderer.
   *
   * \param canvas_tl The top-left position of the canvas.
   * \param canvas_br The bottom-right position of the canvas.
   * \param extent    The current extent of the content.
   * \param tile_size The logical tile size.
   * \param viewport  The state of the viewport.
   */
  CanvasRenderer(const Float2& canvas_tl,
                 const Float2& canvas_br,
                 const MatrixExtent& extent,
                 const Int2& tile_size,
                 const CViewport& viewport);

  ~CanvasRenderer() noexcept;

  /**
   * Clears the canvas with a given color.
   *
   * \param color The color to use.
   */
  void clear_canvas(const UColor& color) const;

  /**
   * Renders an orthogonal grid over the canvas.
   *
   * \param color The line color.
   */
  void draw_orthogonal_grid(const UColor& color) const;

  /**
   * Renders an outline around the tile at the specified position.
   *
   * \param world_pos The position of the tile, in world-space.
   * \param color     The outline color.
   */
  void draw_tile_outline(const Float2& world_pos, const UColor& color) const;

  /**
   * Returns the world-space position of the main content.
   *
   * \return
   * The content origin position.
   */
  [[nodiscard]]
  auto get_content_position() const noexcept -> Float2;

  /**
   * Returns the position of the associated canvas.
   *
   * \return
   * A canvas position.
   */
  [[nodiscard]]
  auto get_canvas_position() const noexcept -> Float2;

  /**
   * Returns the size of the associated canvas.
   *
   * \return
   * A canvas size.
   */
  [[nodiscard]]
  auto get_canvas_size() const noexcept -> Float2;

  /**
   * Returns the region that is visible of the content.
   *
   * \return
   * The visible world-space region.
   */
  [[nodiscard]]
  auto get_visible_region() const -> const VisibleRegion&;

  /**
   * Returns the region of visible tiles in the canvas.
   *
   * \note
   * The tiles referenced by the returned region don't necessarily refer to any
   * valid tiles in the associated content.
   *
   * \return
   * A tile region.
   */
  [[nodiscard]]
  auto get_visible_tiles() const noexcept -> const VisibleTileRegion&;

  /**
   * Returns the tiles that are visible and should be rendered.
   *
   * \return
   * The current render bounds.
   */
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

  /**
   * Returns the canvas scale factor.
   *
   * \return
   * A scale factor.
   */
  [[nodiscard]]
  auto get_scale() const -> float;

  /**
   * Converts a screen-space position to the corresponding world-space position.
   *
   * \param screen_pos A screen-space position.
   *
   * \return
   * A world-space position.
   */
  [[nodiscard]]
  auto to_world_pos(const Float2& screen_pos) const noexcept -> Float2;

  /**
   * Converts a world-space position to the corresponding screen-space position.
   *
   * \param world_pos A world-space position.
   *
   * \return
   * A screen-space position.
   */
  [[nodiscard]]
  auto to_screen_pos(const Float2& world_pos) const noexcept -> Float2;

  [[nodiscard]]
  auto to_screen_pos(const MatrixIndex& tile_pos) const noexcept -> Float2;

  /**
   * Returns the draw list associated with the window.
   *
   * \return
   * The window draw list.
   */
  [[nodiscard]]
  static auto get_draw_list() noexcept -> ImDrawList&;

 private:
  /** The content extent. */
  MatrixExtent mExtent;

  /** The viewport position relative to the content origin (world-space). */
  Float2 mViewportPos;

  /** The size of rendered tiles. */
  Float2 mCanvasTileSize;

  /** The top-left window corner position (screen-space). */
  Float2 mWindowTL;

  /** The bottom-right window corner position (screen-space). */
  Float2 mWindowBR;

  /** The scale factor of rendered content. */
  float mScale;

  /** The region visible via the current viewport. */
  VisibleRegion mVisibleRegion;

  /** The tile region visible via the current viewport. */
  VisibleTileRegion mVisibleTiles;

  /** The content tile positions that are within the visible region. */
  RenderBounds mRenderBounds;
};

}  // namespace ui
}  // namespace tactile

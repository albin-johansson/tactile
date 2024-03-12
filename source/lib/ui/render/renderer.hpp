// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/region.hpp"
#include "core/tile/tile_pos.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Map)
TACTILE_FWD_DECLARE_CLASS(Tileset)
TACTILE_FWD_DECLARE_CLASS(Layer)
TACTILE_FWD_DECLARE_CLASS(TileLayer)
TACTILE_FWD_DECLARE_CLASS(ObjectLayer)
TACTILE_FWD_DECLARE_CLASS(Object)
TACTILE_FWD_DECLARE_CLASS(Color)
TACTILE_FWD_DECLARE_CLASS(Viewport)

/// Provides useful information about a canvas for use by renderer instances.
struct CanvasInfo final {
  Float2 top_left {};      ///< Top-left point of the canvas.
  Float2 bottom_right {};  ///< Bottom-right point of the canvas.
  Float2 size {};          ///< Total size of the canvas.
  Region bounds;           ///< The region of the content that that should be rendered.
  Float2 origin {};        ///< Origin screen position.
  Float2 grid_size {};     ///< Graphical tile size.
  Float2 tile_size {};     ///< Logical tile size.
  Float2 ratio {};         ///< Graphical tile size divided by logical tile size.
  Float2 content_size {};  ///< Graphical size of the map or tileset.
  int32 tiles_in_viewport_x {};  ///< Amount of visible tiles in the viewport, x-axis.
  int32 tiles_in_viewport_y {};  ///< Amount of visible tiles in the viewport, y-axis.
  float row_count {};            ///< Total amount of rows.
  float col_count {};            ///< Total amount of columns.
};

/// Provides the API for rendering various core entities such as tiles and layers.
/// Instances of this class are designed to be short-lived, and created each frame.
class Renderer final {
 public:
  /// Initializes a renderer for a canvas with the specified attributes.
  explicit Renderer(const CanvasInfo& canvas_info);

  /// Initializes a renderer for use with a map.
  Renderer(const Viewport& viewport, const Map& map);

  /// Initializes a renderer for use with a tileset.
  Renderer(const Viewport& viewport, const Tileset& tileset);

  /// Enables clipping for the visible canvas area.
  void push_clip() const;

  /// Disables canvas clipping.
  void pop_clip() const;

  void clear(const Color& color) const;

  /// Renders an "infinite" grid over the target canvas.
  void render_infinite_grid(const Color& color) const;

  void render_translated_grid(const Color& color) const;

  /// Renders an outline around the target canvas.
  void render_outline(const Color& color) const;

  /// Renders a single tile from a tileset.
  ///
  /// \param tileset the source tileset.
  /// \param tile_position the position of the tile in the tileset.
  /// \param position the rendered position of the tile.
  /// \param opacity the opacity of the tile, in the interval [0, 255].
  void render_tile(const Tileset& tileset,
                   const TilePos& tile_position,
                   const Float2& position,
                   float opacity = 1.0f) const;

  /// Renders a single tile from a tileset.
  ///
  /// \param tileset the parent tileset of the tile.
  /// \param tile_index index of the tile in the tileset that will be rendered.
  /// \param pos the position of the rendered tile.
  /// \param opacity the opacity of the rendered tile.
  void render_tile(const Tileset& tileset,
                   TileIndex tile_index,
                   const TilePos& pos,
                   float opacity = 1.0f) const;

  /// Renders a single tile within a map.
  ///
  /// \param map the map hosting the tile.
  /// \param tile_id the global tile identifier associated of the tile to render.
  /// \param pos the position of the rendered tile.
  /// \param opacity the opacity of the rendered tile.
  void render_tile(const Map& map,
                   TileID tile_id,
                   const TilePos& pos,
                   float opacity = 1.0f) const;

  /// Renders all tiles in a tile layer.
  void render_tile_layer(const Map& map,
                         const TileLayer& layer,
                         float parent_opacity) const;

  /// Renders all objects in an object layer.
  void render_object_layer(const ObjectLayer& layer, float parent_opacity) const;

  /// Renders a single map object.
  void render_object(const Object& object, const Color& color) const;

  /// Renders a full map by rendering the contents of all layers.
  void render_map(const Map& map) const;

  /// Renders a full tileset by rendering each contained tile.
  void render_tileset(const Tileset& tileset) const;

  [[nodiscard]] auto get_canvas_info() const -> const CanvasInfo& { return mCanvas; }

  [[nodiscard]] auto translate(const Float2 position) const -> Float2
  {
    return mCanvas.origin + position;
  }

 private:
  CanvasInfo mCanvas;
  Float4 mViewportRect;

  void render_layers(const Map& map) const;

  void render_layer(const Map& map, const Layer& layer, float parent_opacity) const;

  void render_point_object(const Object& object,
                           const Float2& position,
                           const Color& color) const;

  void render_rectangle_object(const Object& object,
                               const Float2& position,
                               const Color& color) const;

  void render_ellipse_object(const Object& object,
                             const Float2& position,
                             const Color& color) const;

  [[nodiscard]] auto from_matrix_to_absolute(TilePos pos) const -> Float2;

  [[nodiscard]] auto is_intersecting_viewport(Float2 position, Float2 size) const -> bool;
  [[nodiscard]] auto is_within_viewport(Float2 position) const -> bool;
};

}  // namespace tactile

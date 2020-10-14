#pragma once

#include <QGraphicsItem>
#include <optional>  // optional

#include "map_document.hpp"
#include "model.hpp"
#include "tileset.hpp"

namespace tactile::gui {

struct render_bounds final
{
  core::row_t rowBegin;
  core::row_t rowEnd;
  core::col_t colBegin;
  core::col_t colEnd;
};

struct render_settings final
{
  render_bounds bounds;
  int tileSize;
  bool drawGrid;
};

/**
 * @class map_item
 *
 * @brief Responsible for the visual representation of maps.
 *
 * @since 0.1.0
 *
 * @headerfile map_item.hpp
 */
class map_item final : public QGraphicsItem
{
 public:
  /**
   * @brief Creates a map item.
   *
   * @param map the associated map document, cannot be null.
   * @param parent the parent item.
   *
   * @throws tactile_error if the supplied map document is null.
   *
   * @since 0.1.0
   */
  explicit map_item(core::map_document* map, QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  void disable_stamp_preview();

  void enable_stamp_preview(const core::position& position);

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 private:
  core::map_document* m_map{};
  std::optional<core::position> m_mousePosition;

  inline constexpr static double m_previewOpacity{0.5};

  [[nodiscard]] auto make_settings(const QRectF& exposed, int tileSize)
      -> render_settings;

  void draw_background(QPainter& painter, const render_settings& bounds);

  void draw_layer(QPainter& painter,
                  const core::layer& layer,
                  const render_settings& bounds);

  void draw_tile(QPainter& painter, tile_id tile, int x, int y, int tileSize);

  void draw_preview(QPainter& painter, int tileSize);

  void draw_preview_multiple_tiles(QPainter& painter,
                                   const core::position& mousePosition,
                                   const core::tileset::selection& selection,
                                   int tileSize);

  template <std::invocable<core::position> T>
  void each_tile(QPainter& painter, const render_bounds& bounds, T&& callable)
  {
    for (auto row = bounds.rowBegin; row < bounds.rowEnd; ++row) {
      for (auto col = bounds.colBegin; col < bounds.colEnd; ++col) {
        const core::position position{row, col};
        callable(position);
      }
    }
  }
};

}  // namespace tactile::gui

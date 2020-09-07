#pragma once

#include <qgraphicsitem.h>

#include "fwd.hpp"
#include "model.hpp"

namespace tactile::gui {

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
   * @param map the associated map, cannot be null.
   * @param tilesets the associated tileset model, cannot be null.
   * @param parent the parent item.
   *
   * @since 0.1.0
   */
  explicit map_item(not_null<core::map*> map,
                    not_null<core::tileset_model*> tilesets,
                    QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 private:
  core::map* m_map{};
  core::tileset_model* m_tilesets{};

  void draw_layer(QPainter& painter,
                  const core::layer& layer,
                  const QRectF& exposed);

  void draw_tile(QPainter& painter, tile_id tile, int x, int y, int tileSize);
};

}  // namespace tactile::gui

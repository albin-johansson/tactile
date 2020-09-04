#pragma once

#include <qgraphicsitem.h>

#include "core_model.hpp"
#include "fwd.hpp"

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
   * @param parent the parent item.
   *
   * @since 0.1.0
   */
  explicit map_item(not_null<core::map*> map,
                    QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 private:
  core::map* m_map{};

  void draw_layer(QPainter& painter,
                  const core::layer& layer,
                  const QRectF& exposed);
};

}  // namespace tactile::gui

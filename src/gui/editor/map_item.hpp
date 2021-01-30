#pragma once

#include <QGraphicsItem>  // QGraphicsItem

#include "maybe.hpp"
#include "position.hpp"
#include "tile_id.hpp"

namespace tactile {

namespace core {
class map_document;
}  // namespace core

namespace gui {

/**
 * \class map_item
 *
 * \brief Responsible for the visual representation of maps.
 *
 * \since 0.1.0
 *
 * \headerfile map_item.hpp
 */
class map_item final : public QGraphicsItem
{
 public:
  /**
   * \brief Creates a map item.
   *
   * \param map the associated map document, cannot be null.
   * \param parent the parent item.
   *
   * \throws tactile_error if the supplied map document is null.
   *
   * \since 0.1.0
   */
  explicit map_item(core::map_document* map, QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  void disable_stamp_preview();

  void enable_stamp_preview(const core::position& position);

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 private:
  core::map_document* m_document{};
  maybe<core::position> m_mousePosition;
};

}  // namespace gui
}  // namespace tactile

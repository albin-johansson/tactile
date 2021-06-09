#pragma once

#include <QGraphicsItem>  // QGraphicsItem

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "position.hpp"
#include "tile_id.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile {

/**
 * \class MapItem
 *
 * \brief Responsible for the visual representation of maps.
 *
 * \since 0.1.0
 *
 * \headerfile map_item.hpp
 */
class MapItem final : public QGraphicsItem
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
  explicit MapItem(core::map_document* map, QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  void EnableStampPreview(const core::position& position);

  void DisableStampPreview();

  void ShowMapProperties();

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 private:
  core::map_document* mDocument{};
  maybe<core::position> mMousePosition;
};

}  // namespace tactile

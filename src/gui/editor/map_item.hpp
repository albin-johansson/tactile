#pragma once

#include <QGraphicsItem>  // QGraphicsItem

#include "fwd.hpp"
#include "map_position.hpp"
#include "maybe.hpp"
#include "tactile_declare_ui.hpp"
#include "tile_id.hpp"

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
   * \throws TactileError if the supplied map document is null.
   *
   * \since 0.1.0
   */
  explicit MapItem(core::MapDocument* map, QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  void EnableStampPreview(const core::MapPosition& position);

  void DisableStampPreview();

  void ShowMapProperties();

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 private:
  core::MapDocument* mDocument{};
  Maybe<core::MapPosition> mMousePosition;
};

}  // namespace tactile

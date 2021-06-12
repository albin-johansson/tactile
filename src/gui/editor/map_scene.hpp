#pragma once

#include <QGraphicsScene>  // QGraphicsScene
#include <QPainter>        // QPainter
#include <QRect>           // QRect

#include "forward_declare.hpp"
#include "map_id.hpp"
#include "position.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile {

class MapItem;

class MapScene final : public QGraphicsScene
{
  Q_OBJECT

 public:
  explicit MapScene(core::MapDocument* map,
                    map_id id,
                    QObject* parent = nullptr);

  /// Moves the viewport by the specified amount.
  void MoveViewport(int dx, int dy);

  /// Enables the stamp tool preview at the specified position.
  void EnableStampPreview(const core::Position& position);

  /// Disables the stamp tool preview.
  void DisableStampPreview();

  /// Makes the properties widget show information about the associated map.
  void ShowMapProperties();

  /// Resets the viewport scale to its default value
  void ResetScale();

  /// Increases the viewport scale by 10%
  void IncreaseScale();

  /// Decreases the viewport scale by 10%
  void DecreaseScale();

  void SetScale(qreal scale);

  /// Returns the current position of the associated map
  [[nodiscard]] auto CurrentMapPosition() const -> QPointF;

  [[nodiscard]] auto MapBounds() const -> QRectF;

  /// Returns the ID of the associated map
  [[nodiscard]] auto Id() const noexcept -> map_id
  {
    return mId;
  }

 protected:
  void drawBackground(QPainter* painter, const QRectF& rect) override;

 private:
  MapItem* mItem{};
  map_id mId;
};

}  // namespace tactile

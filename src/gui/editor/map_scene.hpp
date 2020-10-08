#pragma once

#include <qgraphicsscene.h>
#include <qrect.h>
#include <qwidget.h>

#include "fwd.hpp"
#include "map_document.hpp"
#include "model.hpp"

class QPainter;

namespace tactile::gui {

class map_scene final : public QGraphicsScene
{
  Q_OBJECT

 public:
  explicit map_scene(not_null<core::map_document*> map,
                     map_id id,
                     QWidget* parent = nullptr);

  void move_map(int dx, int dy);

  void center_map();

  void enable_stamp_preview(const core::position& position);

  void disable_stamp_preview();

  [[nodiscard]] auto map_position() const -> QPointF;

  [[nodiscard]] auto id() const noexcept -> map_id
  {
    return m_id;
  }

 protected:
  void drawBackground(QPainter* painter, const QRectF& rect) override;

 private:
  map_item* m_item;
  map_id m_id;
};

}  // namespace tactile::gui

#pragma once

#include <QGraphicsView>  // QGraphicsView
#include <QMouseEvent>    // QMouseEvent
#include <QRect>          // QRect

#include "forward_declare.hpp"
#include "map_id.hpp"
#include "position.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)
TACTILE_FORWARD_DECLARE(tactile, MapScene)

namespace tactile {

/**
 * \class MapView
 *
 * \brief Represents the UI component that presents a map.
 *
 * \details This class also handles the mouse input for maps, and delegates
 * mouse events to the model that will be used by the mouse tools.
 *
 * \since 0.1.0
 *
 * \headerfile map_view.hpp
 */
class MapView final : public QGraphicsView
{
  Q_OBJECT

 public:
  explicit MapView(core::MapDocument* map,
                   map_id id,
                   QWidget* parent = nullptr);

  void ForceRedraw();

  void CenterViewport();

  void MoveViewport(int dx, int dy);

  void EnableStampPreview(const core::Position& position);

  void DisableStampPreview();

  void ShowMapProperties();

  [[nodiscard]] auto Id() const -> map_id;

 public slots:
  void SetOpenGlEnabled(bool enabled);

 signals:
  void S_MousePressed(QMouseEvent* event, QPointF mapPosition);
  void S_MouseMoved(QMouseEvent* event, QPointF mapPosition);
  void S_MouseReleased(QMouseEvent* event, QPointF mapPosition);
  void S_MouseEntered(QEvent* event);
  void S_MouseExited(QEvent* event);
  void S_ZoomIn();
  void S_ZoomOut();
  void S_SpawnContextMenu(const QPoint& pos);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void enterEvent(QEnterEvent* event) override;

  void leaveEvent(QEvent* event) override;

  void wheelEvent(QWheelEvent* event) override;

 private:
  QPoint mLastMousePos{};

  [[nodiscard]] auto GetMapScene() -> MapScene*;

  [[nodiscard]] auto GetMapScene() const -> const MapScene*;
};

}  // namespace tactile

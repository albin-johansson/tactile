#include "map_view.hpp"

#include <QApplication>    // QApplication
#include <QMouseEvent>     // QMouseEvent
#include <QOpenGLWidget>   // QOpenGLWidget
#include <QScrollBar>      // QScrollBar
#include <QSurfaceFormat>  // QSurfaceFormat

#include "map_document.hpp"
#include "map_scene.hpp"
#include "preferences.hpp"

namespace tactile {

MapView::MapView(core::MapDocument* map, const map_id id, QWidget* parent)
    : QGraphicsView{parent}
{
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  setAttribute(Qt::WA_StaticContents, true);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  SetOpenGlEnabled(prefs::UseOpenGl().Value());

  //  grabGesture(Qt::PinchGesture);
  setScene(new MapScene{map, id, this});
}

void MapView::ForceRedraw()
{
  scene()->update();
}

void MapView::CenterViewport()
{
  if (auto* scene = GetMapScene())
  {
    const auto bounds = scene->MapBounds();

    const auto scale =
        std::min(width() / bounds.width(), height() / bounds.height()) * 0.95;

    centerOn(bounds.center());
    scene->SetScale(scale);
  }
}

void MapView::MoveViewport(const int dx, const int dy)
{
  if (auto* scene = GetMapScene())
  {
    scene->MoveViewport(dx, dy);
  }
}

void MapView::EnableStampPreview(const core::MapPosition& position)
{
  GetMapScene()->EnableStampPreview(position);
}

void MapView::DisableStampPreview()
{
  GetMapScene()->DisableStampPreview();
}

void MapView::ShowMapProperties()
{
  GetMapScene()->ShowMapProperties();
}

auto MapView::Id() const -> map_id
{
  return GetMapScene()->Id();
}

void MapView::SetOpenGlEnabled(const bool enabled)
{
  if (enabled)
  {
    if (!qobject_cast<QOpenGLWidget*>(viewport()))
    {
      auto* viewport = new QOpenGLWidget{this};
      viewport->setFormat(QSurfaceFormat::defaultFormat());
      setViewport(viewport);
    }

    if (auto* widget = viewport())
    {
      // register mouse events with no pressed buttons
      widget->setMouseTracking(true);
    }
  }
  else
  {
    setViewport(nullptr);
  }
}

void MapView::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);

  if (event->buttons() & Qt::MiddleButton)
  {
    mLastMousePos = event->pos();
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  }

  emit S_MousePressed(event, mapFromScene(GetMapScene()->CurrentMapPosition()));
}

void MapView::mouseMoveEvent(QMouseEvent* event)
{
  QGraphicsView::mouseMoveEvent(event);

  const auto pos = event->pos();
  const auto buttons = event->buttons();

  if (buttons & Qt::MiddleButton)
  {
    MoveViewport(pos.x() - mLastMousePos.x(), pos.y() - mLastMousePos.y());
  }

  mLastMousePos = pos;

  emit S_MouseMoved(event, mapFromScene(GetMapScene()->CurrentMapPosition()));
}

void MapView::mouseReleaseEvent(QMouseEvent* event)
{
  QGraphicsView::mouseReleaseEvent(event);

  if (const auto button = event->button(); button == Qt::MiddleButton)
  {
    QApplication::restoreOverrideCursor();
  }
  else if (button == Qt::RightButton)
  {
    emit S_SpawnContextMenu(mapToGlobal(event->pos()));
  }

  emit S_MouseReleased(event,
                       mapFromScene(GetMapScene()->CurrentMapPosition()));
}

void MapView::enterEvent(QEnterEvent* event)
{
  QWidget::enterEvent(event);
  emit S_MouseEntered(event);
}

void MapView::leaveEvent(QEvent* event)
{
  QWidget::leaveEvent(event);
  emit S_MouseExited(event);
}

void MapView::wheelEvent(QWheelEvent* event)
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    auto pixels = event->pixelDelta();
    const auto degrees = event->angleDelta() / 8;

    if (!pixels.isNull())
    {
      if (pixels.y() > 0)
      {
        emit S_ZoomIn();
      }
      else
      {
        emit S_ZoomOut();
      }
    }
    else if (!degrees.isNull())
    {
      const auto numSteps = degrees / 15;
      if (numSteps.y() > 0)
      {
        GetMapScene()->IncreaseScale();
      }
      else
      {
        GetMapScene()->DecreaseScale();
      }
    }
  }
}

auto MapView::GetMapScene() -> MapScene*
{
  return qobject_cast<MapScene*>(scene());
}

auto MapView::GetMapScene() const -> const MapScene*
{
  return qobject_cast<const MapScene*>(scene());
}

}  // namespace tactile

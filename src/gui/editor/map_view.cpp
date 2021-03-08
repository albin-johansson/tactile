#include "map_view.hpp"

#include <QApplication>
#include <QOpenGLWidget>
#include <QScrollBar>
#include <QSurfaceFormat>

#include "map_document.hpp"
#include "map_scene.hpp"
#include "preferences.hpp"

namespace tactile::gui {

map_view::map_view(core::map_document* map, const map_id id, QWidget* parent)
    : QGraphicsView{parent}
{
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  setAttribute(Qt::WA_StaticContents, true);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  set_opengl_enabled(prefs::gfx::use_opengl().value());

  //  grabGesture(Qt::PinchGesture);
  setScene(new map_scene{map, id, this});
}

void map_view::force_redraw()
{
  scene()->update();
}

void map_view::center_map()
{
  if (auto* scene = get_map_scene()) {
    scene->center_map();
  }
}

void map_view::move_map(const int dx, const int dy)
{
  if (auto* scene = get_map_scene()) {
    scene->move_map(dx, dy);
  }
}

void map_view::enable_stamp_preview(const core::position& position)
{
  get_map_scene()->enable_stamp_preview(position);
}

void map_view::disable_stamp_preview()
{
  get_map_scene()->disable_stamp_preview();
}

void map_view::show_properties()
{
  get_map_scene()->show_properties();
}

auto map_view::id() const -> map_id
{
  return get_map_scene()->id();
}

void map_view::set_opengl_enabled(const bool enabled)
{
  if (enabled) {
    if (!qobject_cast<QOpenGLWidget*>(viewport())) {
      auto* viewport = new QOpenGLWidget{this};
      viewport->setFormat(QSurfaceFormat::defaultFormat());
      setViewport(viewport);
    }

    if (auto* widget = viewport()) {
      // register mouse events with no pressed buttons
      widget->setMouseTracking(true);
    }
  } else {
    setViewport(nullptr);
  }
}

void map_view::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);

  if (event->buttons() & Qt::MiddleButton) {
    m_lastMousePos = event->pos();
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  }

  emit mouse_pressed(event, mapFromScene(get_map_scene()->map_position()));
}

void map_view::mouseMoveEvent(QMouseEvent* event)
{
  QGraphicsView::mouseMoveEvent(event);

  const auto pos = event->pos();
  const auto buttons = event->buttons();

  if (buttons & Qt::MiddleButton) {
    move_map(pos.x() - m_lastMousePos.x(), pos.y() - m_lastMousePos.y());
  }

  m_lastMousePos = pos;

  emit mouse_moved(event, mapFromScene(get_map_scene()->map_position()));
}

void map_view::mouseReleaseEvent(QMouseEvent* event)
{
  QGraphicsView::mouseReleaseEvent(event);

  if (const auto button = event->button(); button == Qt::MiddleButton) {
    QApplication::restoreOverrideCursor();

  } else if (button == Qt::RightButton) {
    emit spawn_context_menu(mapToGlobal(event->pos()));
  }

  emit mouse_released(event, mapFromScene(get_map_scene()->map_position()));
}

void map_view::enterEvent(QEvent* event)
{
  QWidget::enterEvent(event);
  emit mouse_entered(event);
}

void map_view::leaveEvent(QEvent* event)
{
  QWidget::leaveEvent(event);
  emit mouse_exited(event);
}

void map_view::wheelEvent(QWheelEvent* event)
{
  if (event->modifiers() & Qt::ControlModifier) {
    auto pixels = event->pixelDelta();
    const auto degrees = event->angleDelta() / 8;

    if (!pixels.isNull()) {
      if (pixels.y() > 0) {
        emit increase_zoom();
      } else {
        emit decrease_zoom();
      }
    } else if (!degrees.isNull()) {
      const auto numSteps = degrees / 15;
      if (numSteps.y() > 0) {
        emit increase_zoom();
      } else {
        emit decrease_zoom();
      }
    }
  }
}

auto map_view::get_map_scene() -> map_scene*
{
  return qobject_cast<map_scene*>(scene());
}

auto map_view::get_map_scene() const -> const map_scene*
{
  return qobject_cast<const map_scene*>(scene());
}

}  // namespace tactile::gui

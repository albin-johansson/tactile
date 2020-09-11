#include "eraser_tool.hpp"

#include "model.hpp"

namespace tactile {

eraser_tool::eraser_tool(core::model* model) : abstract_tool{model}
{}

void eraser_tool::update_eraser(QMouseEvent* event, const QPointF& mapPosition)
{
  auto* map = get_model()->current_raw_map();
  if (!map) {
    return;
  }

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    if (const auto pos = translate_mouse_position(event->pos(), mapPosition);
        pos) {
      map->set_tile(*pos, empty);
      emit get_model()->redraw();
    }
  }
}

void eraser_tool::pressed(QMouseEvent* event, const QPointF& mapPosition)
{
  update_eraser(event, mapPosition);
}

void eraser_tool::moved(QMouseEvent* event, const QPointF& mapPosition)
{
  update_eraser(event, mapPosition);
}

}  // namespace tactile

#include "eraser_tool.hpp"

#include "model.hpp"

namespace tactile {

eraser_tool::eraser_tool(core::model* model) : abstract_tool{model}
{
  m_oldState.reserve(20);
  m_positions.reserve(20);
}

void eraser_tool::update_eraser(QMouseEvent* event, const QPointF& mapPosition)
{
  auto* map = get_model()->current_raw_map();
  if (!map) {
    return;
  }

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    if (const auto pos = translate_mouse_position(event->pos(), mapPosition);
        pos) {
      if (!m_oldState.contains(*pos)) {
        m_oldState.emplace(*pos, map->tile_at(*pos).value());
      }
      m_positions.push_back(*pos);

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

void eraser_tool::released(QMouseEvent* event, const QPointF& mapPosition)
{
  auto* map = get_model()->current_raw_map();
  if (!map) {
    return;
  }

  if (event->button() == Qt::MouseButton::LeftButton) {
    auto* mapModel = get_model()->current_map_model();
    Q_ASSERT(mapModel);

    mapModel->add_erase_sequence(std::move(m_oldState), std::move(m_positions));

    m_oldState.clear();
    m_positions.clear();
  }
}

}  // namespace tactile

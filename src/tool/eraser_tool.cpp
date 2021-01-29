#include "eraser_tool.hpp"

#include "map_document.hpp"
#include "model.hpp"
#include "tile_layer.hpp"

namespace tactile {

eraser_tool::eraser_tool(core::model* model) : abstract_tool{model}
{
  m_oldState.reserve(20);
}

void eraser_tool::update_eraser(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = get_model()->current_document()) {
    const auto id = document->current_layer_id().value();
    auto* tileLayer = document->get_tile_layer(id);
    Q_ASSERT(tileLayer);

    if (event->buttons() & Qt::MouseButton::LeftButton) {
      const auto pos = translate_mouse_position(event->pos(), mapPosition);
      if (pos) {
        if (!m_oldState.contains(*pos)) {
          m_oldState.emplace(*pos, tileLayer->tile_at(*pos).value());
        }

        tileLayer->set_tile(*pos, empty);
        emit get_model()->redraw();
      }
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
  if (auto* document = get_model()->current_document()) {
    if (event->button() == Qt::MouseButton::LeftButton) {
      document->add_erase_sequence(std::move(m_oldState));
      m_oldState.clear();
    }
  }
}

}  // namespace tactile

#include "stamp_tool.hpp"

#include "model.hpp"

namespace tactile {

using namespace core;

stamp_tool::stamp_tool(model* model) : abstract_tool{model}
{
  m_oldState.reserve(20);
  m_sequence.reserve(20);
}

void stamp_tool::update_stamp_sequence(map_document& map,
                                       const tileset& ts,
                                       const position& origin)
{
  ts.iterate_selection(origin, [&](position mapPos, position tilesetPos) {
    if (map.in_bounds(mapPos)) {
      const auto newID = ts.tile_at(tilesetPos);

      if (!m_oldState.contains(mapPos)) {
        m_oldState.emplace(mapPos, map.tile_at(mapPos).value());
      }
      m_sequence.emplace(mapPos, newID);

      map.set_tile(mapPos, newID);
    }
  });
}

void stamp_tool::pressed(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = get_model()->current_document()) {
    auto* tileset = document->current_tileset();
    if (!tileset || !tileset->get_selection().has_value()) {
      return;
    }

    if (event->buttons() & Qt::MouseButton::LeftButton) {
      const auto pos = translate_mouse_position(event->pos(), mapPosition);
      if (pos) {
        update_stamp_sequence(*document, *tileset, *pos);
        emit get_model()->redraw();
      }
    }
  }
}

void stamp_tool::moved(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = get_model()->current_document()) {
    const auto* tileset = document->current_tileset();
    if (!tileset || !tileset->get_selection()) {
      return;
    }

    const auto pos = translate_mouse_position(event->pos(), mapPosition);
    if (pos) {
      emit get_model()->enable_stamp_preview(*pos);

      if (event->buttons() & Qt::MouseButton::LeftButton) {
        update_stamp_sequence(*document, *tileset, *pos);
      }

      emit get_model()->redraw();
    } else {
      // mouse is outside of map, so disable preview
      emit get_model()->disable_stamp_preview();
    }
  }
}

void stamp_tool::released(QMouseEvent* event, const QPointF&)
{
  if (auto* document = get_model()->current_document()) {
    auto* tileset = document->current_tileset();
    if (!tileset || !tileset->get_selection()) {
      return;
    }

    if (event->button() == Qt::MouseButton::LeftButton) {
      document->add_stamp_sequence(std::move(m_oldState),
                                   std::move(m_sequence));

      // Clearing the maps allows for them to be used after being moved from
      m_oldState.clear();
      m_sequence.clear();
    }
  }
}

void stamp_tool::exited(QEvent*)
{
  emit get_model()->disable_stamp_preview();
}

void stamp_tool::disable()
{
  emit get_model()->disable_stamp_preview();
}

}  // namespace tactile

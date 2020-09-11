#include "stamp_tool.hpp"

#include <optional>

#include "model.hpp"

namespace tactile {

using core::operator""_row;
using core::operator""_col;

stamp_tool::stamp_tool(core::model* model) : abstract_tool{model}
{}

void stamp_tool::apply_current_selection_to_map(const core::position& position,
                                                const core::tileset& tileset)
{
  auto* map = get_model()->current_raw_map();
  Q_ASSERT(map);
  Q_ASSERT(tileset.get_selection());

  const auto& [topLeft, bottomRight] = *tileset.get_selection();

  if (topLeft == bottomRight) {
    map->set_tile(position,
                  tileset.tile_at(topLeft.get_row(), topLeft.get_col()));
  } else {
    const auto nRows = 1_row + (bottomRight.get_row() - topLeft.get_row());
    const auto nCols = 1_col + (bottomRight.get_col() - topLeft.get_col());

    const auto mouseRow = position.get_row();
    const auto mouseCol = position.get_col();

    for (core::row r{0}; r < nRows; ++r) {
      for (core::col c{0}; c < nCols; ++c) {
        const auto tileRow = mouseRow + r;
        const auto tileCol = mouseCol + c;
        if (map->in_bounds(tileRow, tileCol)) {
          map->set_tile(
              {tileRow, tileCol},
              tileset.tile_at(topLeft.get_row() + r, topLeft.get_col() + c));
        }
      }
    }
  }
}

void stamp_tool::pressed(QMouseEvent* event, const QPointF& mapPosition)
{
  auto* tileset = get_model()->current_tileset();
  if (!tileset || !tileset->get_selection()) {
    return;
  }

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    if (const auto pos = translate_mouse_position(event->pos(), mapPosition);
        pos) {
      apply_current_selection_to_map(*pos, *tileset);
      emit get_model()->redraw();
    }
  }
}

void stamp_tool::moved(QMouseEvent* event, const QPointF& mapPosition)
{
  auto* tileset = get_model()->current_tileset();
  if (!tileset || !tileset->get_selection()) {
    return;
  }

  if (const auto pos = translate_mouse_position(event->pos(), mapPosition);
      pos) {
    emit get_model()->enable_stamp_preview(*pos);

    if (event->buttons() & Qt::MouseButton::LeftButton) {
      apply_current_selection_to_map(*pos, *tileset);
    }

    emit get_model()->redraw();
  } else {
    // mouse is outside of map, so disable preview
    emit get_model()->disable_stamp_preview();
  }
}

void stamp_tool::released(QMouseEvent* event, const QPointF& mapPosition)
{
  if (!get_model()->current_tileset()) {
    return;
  }

  if (event->button() == Qt::MouseButton::LeftButton) {
    // TODO we want to be able to undo the changes, create command without
    // executing it.
  }
}

void stamp_tool::exited(QEvent* event)
{
  emit get_model()->disable_stamp_preview();
}

void stamp_tool::disable()
{
  emit get_model()->disable_stamp_preview();
}

}  // namespace tactile

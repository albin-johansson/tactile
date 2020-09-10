#include "stamp_tool.hpp"

#include <optional>

#include "model.hpp"
#include "tactile_error.hpp"

namespace tactile {

using core::position;
using core::tileset;
using core::operator""_row;
using core::operator""_col;

stamp_tool::stamp_tool(core::model* model) : m_model{model}
{
  if (!m_model) {
    throw tactile_error{"Cannot create stamp tool from null model!"};
  }
}

auto stamp_tool::translate_mouse_position(const QPoint& mousePosition,
                                          const QPointF& mapPosition) const
    -> std::optional<core::position>
{
  const auto x = mousePosition.x() - mapPosition.x();
  const auto y = mousePosition.y() - mapPosition.y();

  if (x < 0 || y < 0) {
    return std::nullopt;
  }

  auto* map = m_model->current_raw_map();
  Q_ASSERT(map);

  const auto tileSize = map->get_tile_size().get();

  const core::row row{static_cast<int>(y) / tileSize};
  const core::col col{static_cast<int>(x) / tileSize};

  if (map->in_bounds(row, col)) {
    return core::position{row, col};
  } else {
    return std::nullopt;
  }
}

void stamp_tool::set_tiles(const position& position,
                           const core::tileset& tileset)
{
  auto* map = m_model->current_raw_map();
  Q_ASSERT(map);

  const auto& [topLeft, bottomRight] = tileset.get_selection();

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
  auto* tileset = m_model->current_tileset();
  if (!tileset) {
    return;
  }

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    if (const auto pos = translate_mouse_position(event->pos(), mapPosition);
        pos) {
      set_tiles(*pos, *tileset);
      emit m_model->redraw();
    }
  }
}

void stamp_tool::moved(QMouseEvent* event, const QPointF& mapPosition)
{
  auto* tileset = m_model->current_tileset();
  if (!tileset) {
    return;
  }

  if (const auto pos = translate_mouse_position(event->pos(), mapPosition);
      pos) {
    emit m_model->enable_stamp_preview(*pos);

    if (event->buttons() & Qt::MouseButton::LeftButton) {
      set_tiles(*pos, *tileset);
    }

    emit m_model->redraw();
  } else {
    // mouse is outside of map, so disable preview
    emit m_model->disable_stamp_preview();
  }
}

void stamp_tool::released(QMouseEvent* event, const QPointF& mapPosition)
{
  if (!m_model->current_tileset()) {
    return;
  }

  if (event->button() == Qt::MouseButton::LeftButton) {
    // TODO we want to be able to undo the changes, create command without
    // executing it.
  }
}

void stamp_tool::entered(QEvent* event)
{
  // do nothing
}

void stamp_tool::exited(QEvent* event)
{
  emit m_model->disable_stamp_preview();
}

}  // namespace tactile

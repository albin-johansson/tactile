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

  Q_ASSERT(m_model->has_active_map());
  auto* map = m_model->current_map()->get();

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
  auto* map = m_model->current_map()->get();
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

  auto* map = m_model->current_map()->get();
  if (!map) {
    return;
  }

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    if (const auto pos = translate_mouse_position(event->pos(), mapPosition);
        pos) {
      set_tiles(*pos, *tileset);
      emit m_model->redraw_requested();
    }
  }
}

void stamp_tool::moved(QMouseEvent* event, const QPointF& mapPosition)
{
  auto* tileset = m_model->current_tileset();
  if (!tileset) {
    return;
  }

  if (!m_model->current_tileset()) {
    return;
  }

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    if (const auto pos = translate_mouse_position(event->pos(), mapPosition);
        pos) {
      set_tiles(*pos, *tileset);
      emit m_model->redraw_requested();
    }
  } else {
    // TODO update the preview here, emit signal
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

}  // namespace tactile

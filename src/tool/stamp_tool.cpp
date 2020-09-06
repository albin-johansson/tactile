#include "stamp_tool.hpp"

#include <optional>

#include "model.hpp"
#include "tactile_error.hpp"

namespace tactile {

stamp_tool::stamp_tool(core::model* model) : m_model{model}
{
  if (!m_model) {
    throw tactile_error{"Cannot create stamp tool from null model!"};
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

  const auto get_mouse_position = [&]() -> std::optional<core::position> {
    const auto x = event->x() - mapPosition.x();
    const auto y = event->y() - mapPosition.y();

    if (x < 0 || y < 0) {
      return std::nullopt;
    }

    const auto tileSize = map->get_tile_size().get();

    const core::row row{static_cast<int>(y) / tileSize};
    const core::col col{static_cast<int>(x) / tileSize};

    if (map->in_bounds(row, col)) {
      return core::position{row, col};
    } else {
      return std::nullopt;
    }
  };

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    const auto pos = get_mouse_position();
    if (!pos) {
      return;
    }

    const auto& [topLeft, bottomRight] = tileset->get_selection();

    if (topLeft == bottomRight) {
      map->set_tile(*pos,
                    tileset->tile_at(topLeft.get_row(), topLeft.get_col()));
    } else {
      const auto nRows = bottomRight.get_row() - topLeft.get_row();
      const auto nCols = bottomRight.get_col() - topLeft.get_col();

      const auto mouseRow = pos->get_row();
      const auto mouseCol = pos->get_col();

      for (core::row r{0}; r < nRows; ++r) {
        for (core::col c{0}; c < nCols; ++c) {
          const auto tileRow = mouseRow + r;
          const auto tileCol = mouseCol + c;
          if (map->in_bounds(tileRow, tileCol)) {
            map->set_tile(
                {tileRow, tileCol},
                tileset->tile_at(topLeft.get_row() + r, topLeft.get_col() + c));
          }
        }
      }
    }
  }
}

void stamp_tool::moved(QMouseEvent* event, const QPointF& mapPosition)
{
  if (!m_model->current_tileset()) {
    return;
  }

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    //    qDebug("stamp_tool > moved");

  } else {
    // update the preview here, emit signal
  }
}

void stamp_tool::released(QMouseEvent* event, const QPointF& mapPosition)
{
  if (!m_model->current_tileset()) {
    return;
  }

  if (event->button() == Qt::MouseButton::LeftButton) {
    //    qDebug("stamp_tool > released");
  }
}

// auto stamp_tool::is_single_tile_selected() const -> bool
//{
//  Q_ASSERT(m_model->current_tileset());
//  return m_model->current_tileset()->num_selected() == 1;
//}

}  // namespace tactile

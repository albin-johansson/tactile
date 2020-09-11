#include "bucket_tool.hpp"

#include "model.hpp"

namespace tactile {

bucket_tool::bucket_tool(core::model* model) : abstract_tool{model}
{}

void bucket_tool::pressed(QMouseEvent* event, const QPointF& mapPosition)
{
  auto* map = get_model()->current_raw_map();
  if (!map) {
    return;
  }

  auto* tileset = get_model()->current_tileset();
  if (!tileset || !tileset->get_selection()) {
    return;
  }

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    if (const auto pos = translate_mouse_position(event->pos(), mapPosition);
        pos) {
      if (tileset->is_single_tile_selected()) {
        if (const auto target = map->tile_at(*pos); target) {
          auto* mapModel = get_model()->current_map_model();
          mapModel->flood(*pos,
                          *target,
                          tileset->tile_at(tileset->get_selection()->topLeft));
          emit get_model()->redraw();
        }
      }
    }
  }
}

}  // namespace tactile

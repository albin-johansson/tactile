#include "bucket_tool.hpp"

namespace tactile {

bucket_tool::bucket_tool(core::model* model) : abstract_tool{model}
{}

void bucket_tool::pressed(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = get_model()->current_document()) {
    auto* tileset = document->current_tileset();
    if (!tileset || !tileset->get_selection()) {
      return;
    }

    if (event->buttons() & Qt::MouseButton::LeftButton) {
      const auto pos = translate_mouse_position(event->pos(), mapPosition);
      if (pos && tileset->is_single_tile_selected()) {
        const auto target = document->tile_at(*pos);
        if (target) {
          document->flood(*pos,
                          *target,
                          tileset->tile_at(tileset->get_selection()->topLeft));
          emit get_model()->redraw();
        }
      }
    }
  }
}

}  // namespace tactile

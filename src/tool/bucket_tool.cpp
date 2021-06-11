#include "bucket_tool.hpp"

#include "map_document.hpp"
#include "model.hpp"

namespace tactile {

bucket_tool::bucket_tool(core::Model* model) : abstract_tool{model}
{}

void bucket_tool::pressed(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = get_model()->CurrentDocument())
  {
    auto* tileset = document->CurrentTileset();
    if (!tileset || !tileset->GetSelection())
    {
      return;
    }

    if (event->buttons() & Qt::MouseButton::LeftButton)
    {
      const auto pos = translate_mouse_position(event->pos(), mapPosition);
      if (pos && tileset->IsSingleTileSelected())
      {
        document->Flood(*pos,
                        tileset->TileAt(tileset->GetSelection()->topLeft));
        emit get_model()->S_Redraw();
      }
    }
  }
}

}  // namespace tactile

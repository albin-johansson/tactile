#include "bucket_tool.hpp"

#include "map_document.hpp"
#include "model.hpp"

namespace tactile {

bucket_tool::bucket_tool(core::Model* model) : AMouseTool{model}
{}

void bucket_tool::OnPressed(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = GetModel()->CurrentDocument())
  {
    auto* tileset = document->CurrentTileset();
    if (!tileset || !tileset->GetSelection())
    {
      return;
    }

    if (event->buttons() & Qt::MouseButton::LeftButton)
    {
      const auto pos = TranslateMousePosition(event->pos(), mapPosition);
      if (pos && tileset->IsSingleTileSelected())
      {
        document->Flood(*pos,
                        tileset->TileAt(tileset->GetSelection()->topLeft));
        emit GetModel()->S_Redraw();
      }
    }
  }
}

}  // namespace tactile

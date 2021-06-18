#include "eraser_tool.hpp"

#include <QMouseEvent>  // QMouseEvent

#include "map_document.hpp"
#include "model.hpp"
#include "tile_layer.hpp"

namespace tactile {

EraserTool::EraserTool(core::Model* model) : AMouseTool{model}
{
  mOldState.reserve(20);
}

void EraserTool::UpdateEraser(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = GetModel()->CurrentDocument())
  {
    const auto id = document->CurrentLayerId().value();
    auto* tileLayer = document->GetTileLayer(id);
    Q_ASSERT(tileLayer);

    if (event->buttons() & Qt::MouseButton::LeftButton)
    {
      if (const auto pos = TranslateMousePosition(event->pos(), mapPosition))
      {
        if (!mOldState.contains(*pos))
        {
          mOldState.emplace(*pos, tileLayer->TileAt(*pos).value());
        }

        tileLayer->SetTile(*pos, empty);
        emit GetModel()->S_Redraw();
      }
    }
  }
}

void EraserTool::OnPressed(QMouseEvent* event, const QPointF& mapPosition)
{
  UpdateEraser(event, mapPosition);
}

void EraserTool::OnMoved(QMouseEvent* event, const QPointF& mapPosition)
{
  UpdateEraser(event, mapPosition);
}

void EraserTool::OnReleased(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = GetModel()->CurrentDocument())
  {
    if (event->button() == Qt::MouseButton::LeftButton)
    {
      document->AddEraseSequence(std::move(mOldState));
      mOldState.clear();
    }
  }
}

}  // namespace tactile

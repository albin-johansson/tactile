#include "stamp_tool.hpp"

#include "model.hpp"
#include "tile_layer.hpp"

namespace tactile {

StampTool::StampTool(core::Model* model) : AMouseTool{model}
{
  mOldState.reserve(32);
  mSequence.reserve(32);
}

void StampTool::UpdateStampSequence(core::MapDocument& map,
                                    const core::Tileset& tileset,
                                    const core::Position& origin)
{
  const auto callable = [&](const core::Position& mapPos,
                            const core::Position& tilesetPos) {
    if (map.InBounds(mapPos))
    {
      const auto newID = tileset.TileAt(tilesetPos);

      auto* tileLayer = map.GetTileLayer(map.CurrentLayerId().value());
      Q_ASSERT(tileLayer);

      if (!mOldState.contains(mapPos))
      {
        mOldState.emplace(mapPos, tileLayer->TileAt(mapPos).value());
      }
      mSequence.emplace(mapPos, newID);

      tileLayer->SetTile(mapPos, newID);
    }
  };

  tileset.VisitSelection(origin, callable);
}

void StampTool::OnPressed(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = GetModel()->CurrentDocument())
  {
    auto* tileset = document->CurrentTileset();
    if (!tileset || !tileset->GetSelection().has_value())
    {
      return;
    }

    if (event->buttons() & Qt::MouseButton::LeftButton)
    {
      const auto pos = TranslateMousePosition(event->pos(), mapPosition);
      if (pos)
      {
        UpdateStampSequence(*document, *tileset, *pos);
        emit GetModel()->S_Redraw();
      }
    }
  }
}

void StampTool::OnMoved(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = GetModel()->CurrentDocument())
  {
    const auto* tileset = document->CurrentTileset();
    if (!tileset || !tileset->GetSelection())
    {
      return;
    }

    if (const auto pos = TranslateMousePosition(event->pos(), mapPosition))
    {
      emit GetModel()->S_EnableStampPreview(*pos);

      if (event->buttons() & Qt::MouseButton::LeftButton)
      {
        UpdateStampSequence(*document, *tileset, *pos);
      }

      emit GetModel()->S_Redraw();
    }
    else
    {
      // Mouse is outside of map, so disable preview
      emit GetModel()->S_DisableStampPreview();
    }
  }
}

void StampTool::OnReleased(QMouseEvent* event, const QPointF&)
{
  if (auto* document = GetModel()->CurrentDocument())
  {
    auto* tileset = document->CurrentTileset();
    if (!tileset || !tileset->GetSelection())
    {
      return;
    }

    if (event->button() == Qt::MouseButton::LeftButton)
    {
      document->AddStampSequence(std::move(mOldState), std::move(mSequence));

      // Clearing the maps allows for them to be used after being moved from
      mOldState.clear();
      mSequence.clear();
    }
  }
}

void StampTool::OnExited(QEvent*)
{
  emit GetModel()->S_DisableStampPreview();
}

void StampTool::Disable()
{
  emit GetModel()->S_DisableStampPreview();
}

}  // namespace tactile

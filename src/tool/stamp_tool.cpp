#include "stamp_tool.hpp"

#include "model.hpp"
#include "tile_layer.hpp"

namespace tactile {

using namespace core;

stamp_tool::stamp_tool(Model* model) : AMouseTool{model}
{
  m_oldState.reserve(20);
  m_sequence.reserve(20);
}

void stamp_tool::update_stamp_sequence(MapDocument& map,
                                       const Tileset& ts,
                                       const Position& origin)
{
  const auto callable = [&](const Position& mapPos,
                            const Position& tilesetPos) {
    if (map.InBounds(mapPos))
    {
      const auto newID = ts.TileAt(tilesetPos);

      auto* tileLayer = map.GetTileLayer(map.CurrentLayerId().value());
      Q_ASSERT(tileLayer);

      if (!m_oldState.contains(mapPos))
      {
        m_oldState.emplace(mapPos, tileLayer->TileAt(mapPos).value());
      }
      m_sequence.emplace(mapPos, newID);

      tileLayer->SetTile(mapPos, newID);
    }
  };

  ts.VisitSelection(origin, callable);
}

void stamp_tool::OnPressed(QMouseEvent* event, const QPointF& mapPosition)
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
        update_stamp_sequence(*document, *tileset, *pos);
        emit GetModel()->S_Redraw();
      }
    }
  }
}

void stamp_tool::OnMoved(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = GetModel()->CurrentDocument())
  {
    const auto* tileset = document->CurrentTileset();
    if (!tileset || !tileset->GetSelection())
    {
      return;
    }

    const auto pos = TranslateMousePosition(event->pos(), mapPosition);
    if (pos)
    {
      emit GetModel()->S_EnableStampPreview(*pos);

      if (event->buttons() & Qt::MouseButton::LeftButton)
      {
        update_stamp_sequence(*document, *tileset, *pos);
      }

      emit GetModel()->S_Redraw();
    }
    else
    {
      // mouse is outside of map, so disable preview
      emit GetModel()->S_DisableStampPreview();
    }
  }
}

void stamp_tool::OnReleased(QMouseEvent* event, const QPointF&)
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
      document->AddStampSequence(std::move(m_oldState), std::move(m_sequence));

      // Clearing the maps allows for them to be used after being moved from
      m_oldState.clear();
      m_sequence.clear();
    }
  }
}

void stamp_tool::OnExited(QEvent*)
{
  emit GetModel()->S_DisableStampPreview();
}

void stamp_tool::Disable()
{
  emit GetModel()->S_DisableStampPreview();
}

}  // namespace tactile

#include "eraser_tool.hpp"

#include "map_document.hpp"
#include "model.hpp"
#include "tile_layer.hpp"

namespace tactile {

eraser_tool::eraser_tool(core::Model* model) : AMouseTool{model}
{
  m_oldState.reserve(20);
}

void eraser_tool::update_eraser(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = GetModel()->CurrentDocument())
  {
    const auto id = document->CurrentLayerId().value();
    auto* tileLayer = document->GetTileLayer(id);
    Q_ASSERT(tileLayer);

    if (event->buttons() & Qt::MouseButton::LeftButton)
    {
      const auto pos = TranslateMousePosition(event->pos(), mapPosition);
      if (pos)
      {
        if (!m_oldState.contains(*pos))
        {
          m_oldState.emplace(*pos, tileLayer->TileAt(*pos).value());
        }

        tileLayer->SetTile(*pos, empty);
        emit GetModel()->S_Redraw();
      }
    }
  }
}

void eraser_tool::OnPressed(QMouseEvent* event, const QPointF& mapPosition)
{
  update_eraser(event, mapPosition);
}

void eraser_tool::OnMoved(QMouseEvent* event, const QPointF& mapPosition)
{
  update_eraser(event, mapPosition);
}

void eraser_tool::OnReleased(QMouseEvent* event, const QPointF& mapPosition)
{
  if (auto* document = GetModel()->CurrentDocument())
  {
    if (event->button() == Qt::MouseButton::LeftButton)
    {
      document->AddEraseSequence(std::move(m_oldState));
      m_oldState.clear();
    }
  }
}

}  // namespace tactile

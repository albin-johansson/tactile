#include "bucket_fill.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

bucket_fill::bucket_fill(not_null<core::map_document*> document,
                         const core::position& position,
                         const tile_id replacement)
    : QUndoCommand{TACTILE_QSTRING(u"Bucket Fill")}
    , m_document{document}
    , m_origin{position}
    , m_replacement{replacement}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
  m_positions.reserve(64);
}

void bucket_fill::undo()
{
  QUndoCommand::undo();

  auto& map = m_document->raw();
  const auto layer = map.active_layer_id().value();

  map.select_layer(m_layer);

  auto* tileLayer = map.get_tile_layer(m_layer);
  Q_ASSERT(tileLayer);

  for (const auto& position : m_positions)
  {
    tileLayer->set_tile(position, m_target);
  }

  map.select_layer(layer);
  emit m_document->redraw();
}

void bucket_fill::redo()
{
  QUndoCommand::redo();

  m_positions.clear();

  auto& map = m_document->raw();
  m_layer = map.active_layer_id().value();

  auto* tileLayer = map.get_tile_layer(m_layer);
  Q_ASSERT(tileLayer);

  m_target = tileLayer->tile_at(m_origin).value();
  tileLayer->flood(m_origin, m_replacement, m_positions);

  emit m_document->redraw();
}

}  // namespace tactile::cmd

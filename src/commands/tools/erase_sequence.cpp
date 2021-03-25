#include "erase_sequence.hpp"

#include <utility>  // move

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

erase_sequence::erase_sequence(not_null<core::map_document*> document,
                               vector_map<core::position, tile_id>&& oldState)
    : QUndoCommand{TACTILE_QSTRING(u"Erase Tiles")}
    , m_document{document}
    , m_oldState{std::move(oldState)}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create erase_sequence command from null map!"};
  }
}

void erase_sequence::undo()
{
  QUndoCommand::undo();

  auto& map = m_document->raw();
  const auto layer = map.active_layer_id().value();

  map.select_layer(m_layer);

  auto* tileLayer = map.get_tile_layer(m_layer);
  Q_ASSERT(tileLayer);

  for (const auto& [position, tile] : m_oldState)
  {
    tileLayer->set_tile(position, tile);
  }

  map.select_layer(layer);

  emit m_document->redraw();
}

void erase_sequence::redo()
{
  if (m_first)
  {
    m_layer = m_document->current_layer_id().value();
    m_first = false;
    return;
  }

  QUndoCommand::redo();

  auto& map = m_document->raw();
  const auto layer = map.active_layer_id().value();

  map.select_layer(m_layer);

  auto* tileLayer = map.get_tile_layer(m_layer);
  Q_ASSERT(tileLayer);

  for (const auto& [position, _] : m_oldState)
  {
    tileLayer->set_tile(position, empty);
  }

  map.select_layer(layer);

  emit m_document->redraw();
}

}  // namespace tactile::cmd

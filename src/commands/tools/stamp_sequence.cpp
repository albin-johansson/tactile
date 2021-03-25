#include "stamp_sequence.hpp"

#include <utility>  // move

#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

namespace tactile::cmd {

stamp_sequence::stamp_sequence(not_null<core::map_document*> document,
                               vector_map<core::position, tile_id>&& oldState,
                               vector_map<core::position, tile_id>&& sequence)
    : QUndoCommand{TACTILE_QSTRING(u"Stamp Sequence")}
    , m_document{document}
    , m_oldState{std::move(oldState)}
    , m_sequence{std::move(sequence)}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create stamp_sequence command from null map!"};
  }
}

void stamp_sequence::undo()
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

void stamp_sequence::redo()
{
  // The stamp tools works directly when applied, so we don't do anything when
  // the command is executed when first inserted into the command stack.
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

  for (const auto& [position, tile] : m_sequence)
  {
    tileLayer->set_tile(position, tile);
  }

  map.select_layer(layer);

  emit m_document->redraw();
}

}  // namespace tactile::cmd

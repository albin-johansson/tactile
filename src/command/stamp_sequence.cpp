#include "stamp_sequence.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

stamp_sequence::stamp_sequence(core::map* map,
                               vector_map<core::position, tile_id>&& oldState,
                               vector_map<core::position, tile_id>&& sequence)
    : QUndoCommand{TACTILE_QSTRING(u"Stamp Sequence")}
    , m_map{map}
    , m_oldState{std::move(oldState)}
    , m_sequence{std::move(sequence)}
{
  if (!m_map) {
    throw tactile_error{"Cannot create stamp_sequence command from null map!"};
  }
}

void stamp_sequence::undo()
{
  QUndoCommand::undo();

  const auto layer = m_map->active_layer_id().value();

  m_map->select_layer(m_layer);

  auto* tileLayer = m_map->get_tile_layer(m_layer);
  Q_ASSERT(tileLayer);

  for (const auto& [position, tile] : m_oldState) {
    tileLayer->set_tile(position, tile);
  }

  m_map->select_layer(layer);
}

void stamp_sequence::redo()
{
  // The stamp tools works directly when applied, so we don't do anything when
  // the command is executed when first inserted into the command stack.
  if (m_first) {
    m_layer = m_map->active_layer_id().value();
    m_first = false;
    return;
  }

  QUndoCommand::redo();

  const auto layer = m_map->active_layer_id().value();
  m_map->select_layer(m_layer);

  auto* tileLayer = m_map->get_tile_layer(m_layer);
  Q_ASSERT(tileLayer);

  for (const auto& [position, tile] : m_sequence) {
    tileLayer->set_tile(position, tile);
  }

  m_map->select_layer(layer);
}

}  // namespace tactile::cmd

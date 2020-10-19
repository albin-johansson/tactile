#include "erase_sequence.hpp"

#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::cmd {

erase_sequence::erase_sequence(core::map* map,
                               vector_map<core::position, tile_id>&& oldState)
    : QUndoCommand{QStringLiteral(u"Erase Tiles")},
      m_map{map},
      m_oldState{std::move(oldState)}
{
  if (!m_map) {
    throw tactile_error{"Cannot create erase_sequence command from null map!"};
  }
}

void erase_sequence::undo()
{
  QUndoCommand::undo();

  const auto layer = m_map->active_layer_id().value();

  m_map->select_layer(m_layer);
  for (const auto& [position, tile] : m_oldState) {
    m_map->set_tile(position, tile);
  }

  m_map->select_layer(layer);
}

void erase_sequence::redo()
{
  m_layer = m_map->active_layer_id().value();
  if (m_first) {
    m_first = false;
    return;
  }

  QUndoCommand::redo();

  for (const auto& [position, _] : m_oldState) {
    m_map->set_tile(position, empty);
  }
}

}  // namespace tactile::cmd

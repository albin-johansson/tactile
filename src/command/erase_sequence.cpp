#include "erase_sequence.hpp"

#include <utility>  // move

namespace tactile::cmd {

erase_sequence::erase_sequence(not_null<core::map*> map,
                               vector_map<core::position, tile_id>&& oldState)
    : abstract_command{QStringLiteral(u"Erase Tiles"), map},
      m_oldState{std::move(oldState)}
{}

void erase_sequence::undo()
{
  QUndoCommand::undo();

  for (const auto& [position, tile] : m_oldState) {
    m_map->set_tile(position, tile);
  }
}

void erase_sequence::redo()
{
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

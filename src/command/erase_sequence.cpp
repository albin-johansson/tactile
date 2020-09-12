#include "erase_sequence.hpp"

#include <utility>  // move

namespace tactile::cmd {

erase_sequence::erase_sequence(not_null<core::map*> map,
                               small_map<core::position, tile_id>&& oldState,
                               std::vector<core::position>&& positions)
    : abstract_command{QStringLiteral(u"Erase Tiles"), map},
      m_oldState{std::move(oldState)},
      m_positions{std::move(positions)}
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

  for (const auto& position : m_positions) {
    m_map->set_tile(position, empty);
  }
}

}  // namespace tactile::cmd

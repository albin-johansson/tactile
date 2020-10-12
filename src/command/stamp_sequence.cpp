#include "stamp_sequence.hpp"

#include <utility>  // move

namespace tactile::cmd {

stamp_sequence::stamp_sequence(core::map* map,
                               vector_map<core::position, tile_id>&& oldState,
                               vector_map<core::position, tile_id>&& sequence)
    : abstract_command{QStringLiteral(u"Stamp Sequence"), map},
      m_oldState{std::move(oldState)},
      m_sequence{std::move(sequence)}
{}

void stamp_sequence::undo()
{
  QUndoCommand::undo();

  const auto layer = m_map->active_layer_id();

  m_map->select_layer(m_layer);
  for (const auto& [position, tile] : m_oldState) {
    m_map->set_tile(position, tile);
  }

  m_map->select_layer(layer);
}

void stamp_sequence::redo()
{
  // The stamp tools works directly when applied, so we don't do anything when
  // the command is executed when first inserted into the command stack.
  m_layer = m_map->active_layer_id();
  if (m_first) {
    m_first = false;
    return;
  }

  QUndoCommand::redo();

  for (const auto& [position, tile] : m_sequence) {
    m_map->set_tile(position, tile);
  }
}

}  // namespace tactile::cmd

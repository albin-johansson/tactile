#include "erase_sequence.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

erase_sequence::erase_sequence(core::map* map,
                               vector_map<core::position, tile_id>&& oldState)
    : QUndoCommand{TACTILE_QSTRING(u"Erase Tiles")}
    , m_map{map}
    , m_oldState{std::move(oldState)}
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
  if (m_first) {
    m_layer = m_map->active_layer_id().value();
    m_first = false;
    return;
  }

  QUndoCommand::redo();

  const auto layer = m_map->active_layer_id().value();
  m_map->select_layer(m_layer);
  for (const auto& [position, _] : m_oldState) {
    m_map->set_tile(position, empty);
  }
  m_map->select_layer(layer);
}

}  // namespace tactile::cmd

#include "bucket_fill.hpp"

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

bucket_fill::bucket_fill(core::map* map,
                         const core::position& position,
                         const tile_id replacement)
    : QUndoCommand{TACTILE_QSTRING(u"Bucket Fill")},
      m_map{map},
      m_origin{position},
      m_replacement{replacement}
{
  if (!m_map) {
    throw tactile_error{"Cannot create bucket_fill command from null map!"};
  }
  m_positions.reserve(64);
}

void bucket_fill::undo()
{
  QUndoCommand::undo();

  const auto layer = m_map->active_layer_id().value();

  m_map->select_layer(m_layer);
  for (const auto& position : m_positions) {
    m_map->set_tile(position, m_target);
  }

  m_map->select_layer(layer);
}

void bucket_fill::redo()
{
  QUndoCommand::redo();

  m_positions.clear();

  m_layer = m_map->active_layer_id().value();
  m_target = m_map->tile_at(m_origin).value();
  m_map->flood(m_origin, m_replacement, m_positions);
}

}  // namespace tactile::cmd

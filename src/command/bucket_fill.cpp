#include "bucket_fill.hpp"

namespace tactile::cmd {

bucket_fill::bucket_fill(core::map* map,
                         const core::position& position,
                         tile_id replacement)
    : abstract_command{QStringLiteral(u"Bucket Fill"), map},
      m_origin{position},
      m_replacement{replacement}
{}

void bucket_fill::undo()
{
  QUndoCommand::undo();

  const auto layer = m_map->active_layer_id();

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

  m_layer = m_map->active_layer_id();
  m_target = m_map->tile_at(m_origin).value();
  m_map->flood(m_origin, m_replacement, m_positions);
}

}  // namespace tactile::cmd

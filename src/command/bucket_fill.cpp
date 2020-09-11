#include "bucket_fill.hpp"

namespace tactile::cmd {

bucket_fill::bucket_fill(not_null<core::map*> map,
                         const core::position& position,
                         tile_id target,
                         tile_id replacement)
    : abstract_command{QStringLiteral(u"Flood Fill"), map},
      m_position{position},
      m_target{target},
      m_replacement{replacement}
{}

void bucket_fill::undo()
{
  QUndoCommand::undo();

  // We undo by running a flood fill with inverted target and replacement IDs
  const auto target = m_replacement;
  const auto replacement = m_target;

  m_map->flood(m_position, target, replacement);
}

void bucket_fill::redo()
{
  QUndoCommand::redo();

  m_map->flood(m_position, m_target, m_replacement);
}

}  // namespace tactile::cmd

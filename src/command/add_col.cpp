#include "add_col.hpp"

#include "algorithm.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

add_col::add_col(core::map* map)
    : repeated_command{QStringLiteral(u"Add Column")},
      m_map{map}
{
  if (!m_map) {
    throw tactile_error{"Cannot create command from null map!"};
  }
}

void add_col::undo()
{
  QUndoCommand::undo();

  invoke_n(amount(), [this] { m_map->remove_col(); });
}

void add_col::redo()
{
  QUndoCommand::redo();

  invoke_n(amount(), [this] { m_map->add_col(empty); });
}

}  // namespace tactile::cmd

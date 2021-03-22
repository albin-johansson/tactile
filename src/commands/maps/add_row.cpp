#include "add_row.hpp"

#include "algorithm.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

add_row::add_row(core::map* map)
    : repeated_command{TACTILE_QSTRING(u"Add Row")}
    , m_map{map}
{
  if (!m_map)
  {
    throw tactile_error{"Cannot create command from null map!"};
  }
}

void add_row::undo()
{
  QUndoCommand::undo();
  invoke_n(amount(), [this] {
    m_map->remove_row();
  });
}

void add_row::redo()
{
  QUndoCommand::redo();
  invoke_n(amount(), [this] {
    m_map->add_row(empty);
  });
}

}  // namespace tactile::cmd

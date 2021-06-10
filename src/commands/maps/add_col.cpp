#include "add_col.hpp"

#include "algorithm.hpp"
#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

add_col::add_col(not_null<core::map_document*> document)
    : repeated_command{TACTILE_QSTRING(u"Add Column")}
    , m_document{document}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void add_col::undo()
{
  QUndoCommand::undo();

  invoke_n(amount(), [this] { m_document->raw().remove_col(); });

  emit m_document->redraw();
}

void add_col::redo()
{
  QUndoCommand::redo();

  invoke_n(amount(), [this] { m_document->raw().add_col(empty); });

  emit m_document->redraw();
}

}  // namespace tactile::cmd

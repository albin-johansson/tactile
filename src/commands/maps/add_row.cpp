#include "add_row.hpp"

#include "algorithm.hpp"
#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

add_row::add_row(not_null<core::map_document*> document)
    : repeated_command{TACTILE_QSTRING(u"Add Row")}
    , m_document{document}
{
  if (!m_document)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void add_row::undo()
{
  QUndoCommand::undo();

  invoke_n(amount(), [this] { m_document->raw().remove_row(); });

  emit m_document->redraw();
}

void add_row::redo()
{
  QUndoCommand::redo();

  invoke_n(amount(), [this] { m_document->raw().add_row(empty); });

  emit m_document->redraw();
}

}  // namespace tactile::cmd

#include "add_col.hpp"

#include "algorithm.hpp"
#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

AddCol::AddCol(not_null<core::map_document*> document)
    : repeated_command{TACTILE_QSTRING(u"Add Column")}
    , mDocument{document}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void AddCol::undo()
{
  QUndoCommand::undo();

  invoke_n(amount(), [this] { mDocument->raw().remove_col(); });

  emit mDocument->redraw();
}

void AddCol::redo()
{
  QUndoCommand::redo();

  invoke_n(amount(), [this] { mDocument->raw().add_col(empty); });

  emit mDocument->redraw();
}

}  // namespace tactile::cmd

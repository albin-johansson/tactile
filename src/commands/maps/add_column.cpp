#include "add_column.hpp"

#include "algorithm.hpp"
#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

AddColumn::AddColumn(not_null<core::map_document*> document)
    : RepeatedCommand{TACTILE_QSTRING(u"Add Column")}
    , mDocument{document}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void AddColumn::undo()
{
  QUndoCommand::undo();

  invoke_n(Amount(), [this] { mDocument->raw().remove_col(); });

  emit mDocument->redraw();
}

void AddColumn::redo()
{
  QUndoCommand::redo();

  invoke_n(Amount(), [this] { mDocument->raw().add_col(empty); });

  emit mDocument->redraw();
}

}  // namespace tactile::cmd

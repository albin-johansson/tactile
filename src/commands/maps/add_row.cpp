#include "add_row.hpp"

#include "algorithm.hpp"
#include "map_document.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

AddRow::AddRow(not_null<core::map_document*> document)
    : RepeatedCommand{TACTILE_QSTRING(u"Add Row")}
    , mDocument{document}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create command from null document!"};
  }
}

void AddRow::undo()
{
  QUndoCommand::undo();

  InvokeN(Amount(), [this] { mDocument->raw().remove_row(); });

  emit mDocument->redraw();
}

void AddRow::redo()
{
  QUndoCommand::redo();

  InvokeN(Amount(), [this] { mDocument->raw().add_row(empty); });

  emit mDocument->redraw();
}

}  // namespace tactile::cmd

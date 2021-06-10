#include "rename_property.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

RenameProperty::RenameProperty(core::IPropertyManager* manager,
                               QString oldName,
                               QString newName)
    : QUndoCommand{TACTILE_QSTRING(u"Rename Property \"") + oldName +
                   TACTILE_QSTRING(u"\" to \"") + newName +
                   TACTILE_QSTRING(u"\"")}
    , mManager{manager}
    , mOldName{std::move(oldName)}
    , mNewName{std::move(newName)}
{
  if (!mManager)
  {
    throw TactileError{"Null property manager!"};
  }
}

void RenameProperty::undo()
{
  QUndoCommand::undo();
  mManager->RenameProperty(mNewName, mOldName);
}

void RenameProperty::redo()
{
  QUndoCommand::redo();
  mManager->RenameProperty(mOldName, mNewName);
}

}  // namespace tactile::cmd

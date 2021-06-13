#include "rename_property.hpp"

#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::cmd {

RenameProperty::RenameProperty(core::IPropertyManager* manager,
                               QString oldName,
                               QString newName)
    : QUndoCommand{QStringLiteral(u"Rename Property \"") + oldName +
                   QStringLiteral(u"\" to \"") + newName +
                   QStringLiteral(u"\"")}
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

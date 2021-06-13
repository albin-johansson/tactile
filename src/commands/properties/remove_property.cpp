#include "remove_property.hpp"

#include <utility>  // move

#include "property_manager.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

RemoveProperty::RemoveProperty(core::IPropertyManager* manager, QString name)
    : QUndoCommand{QStringLiteral(u"Remove Property \"") + name +
                   QStringLiteral(u"\"")}
    , mManager{manager}
    , mName{std::move(name)}
{
  if (!mManager)
  {
    throw TactileError{"Cannot create RemoveProperty from null manager!"};
  }
}

void RemoveProperty::undo()
{
  QUndoCommand::undo();

  const auto& property = mProperty.value();
  mManager->AddProperty(mName, property.Type().value());
  mManager->SetProperty(mName, property);

  mProperty.reset();
}

void RemoveProperty::redo()
{
  QUndoCommand::redo();

  mProperty = mManager->GetProperty(mName);
  mManager->RemoveProperty(mName);
}

}  // namespace tactile::cmd

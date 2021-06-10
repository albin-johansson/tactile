#include "remove_property.hpp"

#include <utility>  // move

#include "property_manager.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

RemoveProperty::RemoveProperty(core::property_manager* manager, QString name)
    : QUndoCommand{TACTILE_QSTRING(u"Remove Property \"") + name +
                   TACTILE_QSTRING(u"\"")}
    , mManager{manager}
    , mName{std::move(name)}
{
  if (!mManager)
  {
    throw TactileError{"Cannot create remove_property from null manager!"};
  }
}

void RemoveProperty::undo()
{
  QUndoCommand::undo();

  const auto& property = mProperty.value();
  mManager->add_property(mName, property.type().value());
  mManager->set_property(mName, property);

  mProperty.reset();
}

void RemoveProperty::redo()
{
  QUndoCommand::redo();

  mProperty = mManager->get_property(mName);
  mManager->remove_property(mName);
}

}  // namespace tactile::cmd

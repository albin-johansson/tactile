#include "add_property.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

AddProperty::AddProperty(not_null<core::IPropertyManager*> manager,
                         QString name,
                         data_type data)
    : QUndoCommand{TACTILE_QSTRING(u"Add Property \"") + name +
                   TACTILE_QSTRING(u"\"")}
    , mManager{manager}
    , mName{std::move(name)}
    , mData{std::move(data)}
{
  if (!mManager)
  {
    throw TactileError{"Null property manager!"};
  }
}

AddProperty::AddProperty(not_null<core::IPropertyManager*> manager,
                         QString name,
                         const core::property& property)
    : AddProperty{manager, std::move(name), data_type{property}}
{}

AddProperty::AddProperty(not_null<core::IPropertyManager*> manager,
                         QString name,
                         const core::property_type type)
    : AddProperty{manager, std::move(name), data_type{type}}
{}

void AddProperty::undo()
{
  QUndoCommand::undo();
  mManager->RemoveProperty(mName);
}

void AddProperty::redo()
{
  QUndoCommand::redo();

  if (const auto* type = std::get_if<core::property_type>(&mData))
  {
    mManager->AddProperty(mName, *type);
  }
  else
  {
    mManager->AddProperty(mName, std::get<core::property>(mData));
  }
}

}  // namespace tactile::cmd

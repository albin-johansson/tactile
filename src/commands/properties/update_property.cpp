#include "update_property.hpp"

#include <utility>  // move

#include "property_manager.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

UpdateProperty::UpdateProperty(core::IPropertyManager* manager,
                               QString name,
                               core::property property)
    : QUndoCommand{TACTILE_QSTRING(u"Update Property \"") + name +
                   TACTILE_QSTRING(u"\"")}
    , mManager{manager}
    , mName{std::move(name)}
    , mProperty{std::move(property)}
{
  if (!mManager)
  {
    throw TactileError{"Null property manager!"};
  }
}

void UpdateProperty::undo()
{
  QUndoCommand::undo();

  mManager->SetProperty(mName, mPrevProperty.value());
  mPrevProperty.reset();
}

void UpdateProperty::redo()
{
  QUndoCommand::redo();

  mPrevProperty = mManager->GetProperty(mName);
  mManager->SetProperty(mName, mProperty);
}

}  // namespace tactile::cmd

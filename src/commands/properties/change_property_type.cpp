#include "change_property_type.hpp"

#include <utility>  // move

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

// clang-format off

ChangePropertyType::ChangePropertyType(not_null<core::IPropertyManager*> manager,
                                       QString name,
                                       const core::property_type type)
    : QUndoCommand{TACTILE_QSTRING(u"Change Property Type")}
    , mManager{manager}
    , mName{std::move(name)}
    , mType{type}
{
  if (!mManager) {
    throw TactileError{"Null property manager!"};
  }
}

// clang-format on

void ChangePropertyType::undo()
{
  QUndoCommand::undo();

  const auto& prev = mPreviousProperty.value();
  mManager->ChangePropertyType(mName, prev.type().value());
  mManager->SetProperty(mName, prev);
  mPreviousProperty.reset();
}

void ChangePropertyType::redo()
{
  QUndoCommand::redo();

  mPreviousProperty = mManager->GetProperty(mName);
  mManager->ChangePropertyType(mName, mType);
}

}  // namespace tactile::cmd

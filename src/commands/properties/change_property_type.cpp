#include "change_property_type.hpp"

#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::cmd {

// clang-format off

ChangePropertyType::ChangePropertyType(NotNull<core::IPropertyManager*> manager,
                                       QString name,
                                       const core::PropertyType type)
    : QUndoCommand{QStringLiteral(u"Change Property Type")}
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
  mManager->ChangePropertyType(mName, prev.Type().value());
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

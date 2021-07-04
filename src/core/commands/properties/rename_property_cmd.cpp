#include "rename_property_cmd.hpp"

#include <utility>  // move

#include "core/properties/property_context.hpp"

namespace Tactile {

RenamePropertyCmd::RenamePropertyCmd(NotNull<IPropertyContext*> context,
                                     std::string oldName,
                                     std::string newName)
    : APropertyCommand{context, "Rename Property"}
    , mOldName{std::move(oldName)}
    , mNewName{std::move(newName)}
{}

void RenamePropertyCmd::Undo()
{
  GetContext().RenameProperty(mNewName, mOldName);
}

void RenamePropertyCmd::Redo()
{
  GetContext().RenameProperty(mOldName, mNewName);
}

}  // namespace Tactile

#pragma once

#include <string>  // string

#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/properties/property_command.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class IPropertyContext;

class RenamePropertyCmd final : public APropertyCommand
{
 public:
  RenamePropertyCmd(NotNull<IPropertyContext*> context,
                    std::string oldName,
                    std::string newName);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RenameProperty;
  }

 private:
  std::string mOldName;
  std::string mNewName;
};

/// \} End of group core

}  // namespace Tactile

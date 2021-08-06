#pragma once

#include <string>  // string

#include "aliases/maybe.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/properties/property_command.hpp"
#include "core/properties/property.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class IPropertyContext;

/// \addtogroup commands
/// \{

class RemovePropertyCmd final : public APropertyCommand
{
 public:
  RemovePropertyCmd(NotNull<IPropertyContext*> context, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveProperty;
  }

 private:
  std::string mName;
  Maybe<Property> mProperty;
};

/// \} End of group commands

/// \} End of group core

}  // namespace Tactile

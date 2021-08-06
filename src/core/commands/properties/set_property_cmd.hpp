#pragma once

#include <string>  // string

#include "aliases/maybe.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/properties/property_command.hpp"
#include "core/properties/property.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class IPropertyContext;

/// \addtogroup commands
/// \{

class SetPropertyCmd final : public APropertyCommand
{
 public:
  SetPropertyCmd(NotNull<IPropertyContext*> context,
                 std::string name,
                 Property value);

  void Undo() override;

  void Redo() override;

  auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetProperty;
  }

 private:
  std::string mName;
  Property mValue;
  Maybe<Property> mPrevious;
};

/// \} End of group commands

/// \} End of group core

}  // namespace Tactile

#pragma once

#include <string>  // string

#include "aliases/maybe.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/properties/property_command.hpp"
#include "core/properties/property.hpp"
#include "core/properties/property_type.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

/// \addtogroup commands
/// \{

class ChangePropertyTypeCmd final : public APropertyCommand
{
 public:
  ChangePropertyTypeCmd(NotNull<IPropertyContext*> context,
                        std::string name,
                        PropertyType type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::ChangePropertyType;
  }

 private:
  std::string mName;
  PropertyType mType;
  Maybe<Property> mPreviousValue;
};

/// \} End of group commands

/// \} End of group core

}  // namespace Tactile

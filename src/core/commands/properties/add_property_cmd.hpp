#pragma once

#include <string>  // string

#include "aliases/not_null.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/properties/property_type.hpp"

namespace Tactile {

class IPropertyContext;

class AddPropertyCmd final : public ACommand
{
 public:
  AddPropertyCmd(NotNull<IPropertyContext*> context,
                 std::string name,
                 PropertyType type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddProperty;
  }

 private:
  IPropertyContext* mContext{};
  std::string mName;
  PropertyType mType;
};

}  // namespace Tactile

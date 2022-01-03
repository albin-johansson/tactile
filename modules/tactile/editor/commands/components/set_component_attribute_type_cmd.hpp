#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class SetComponentAttributeTypeCmd final : public ACommand {
 public:
  SetComponentAttributeTypeCmd(RegistryRef registry,
                               ComponentID id,
                               std::string attribute,
                               PropertyType type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetComponentAttributeType;
  }

 private:
  RegistryRef mRegistry;
  ComponentID mComponentId;
  std::string mAttributeName;
  PropertyType mNewType;
  Maybe<PropertyValue> mPreviousValue;
};

}  // namespace Tactile

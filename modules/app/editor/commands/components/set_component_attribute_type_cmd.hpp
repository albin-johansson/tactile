#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class SetComponentAttributeTypeCmd final : public ACommand {
 public:
  SetComponentAttributeTypeCmd(RegistryRef registry,
                               component_id id,
                               std::string attribute,
                               attribute_type type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetComponentAttributeType;
  }

 private:
  RegistryRef mRegistry;
  component_id mComponentId;
  std::string mAttributeName;
  attribute_type mNewType;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile

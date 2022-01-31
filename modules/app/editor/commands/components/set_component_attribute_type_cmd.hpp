#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class SetComponentAttributeTypeCmd final : public command_base {
 public:
  SetComponentAttributeTypeCmd(registry_ref registry,
                               component_id id,
                               std::string attribute,
                               attribute_type type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::SetComponentAttributeType;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId;
  std::string mAttributeName;
  attribute_type mNewType;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile

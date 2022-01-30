#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class RemoveComponentAttributeCmd final : public ACommand {
 public:
  RemoveComponentAttributeCmd(RegistryRef registry,
                              component_id componentId,
                              std::string attribute);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveComponentAttribute;
  }

 private:
  RegistryRef mRegistry;
  component_id mComponentId{};
  std::string mAttributeName;
  maybe<attribute_value> mPreviousDefault;
};

}  // namespace tactile
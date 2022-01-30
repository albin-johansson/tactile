#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class RemoveComponentAttributeCmd final : public ACommand {
 public:
  RemoveComponentAttributeCmd(RegistryRef registry,
                              ComponentID componentId,
                              std::string attribute);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveComponentAttribute;
  }

 private:
  RegistryRef mRegistry;
  ComponentID mComponentId{};
  std::string mAttributeName;
  Maybe<attribute_value> mPreviousDefault;
};

}  // namespace tactile
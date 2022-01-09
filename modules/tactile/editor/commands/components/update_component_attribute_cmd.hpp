#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class UpdateComponentAttributeCmd final : public ACommand {
 public:
  UpdateComponentAttributeCmd(RegistryRef registry,
                              ComponentID id,
                              std::string attribute,
                              PropertyValue value);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::UpdateComponentAttribute;
  }

 private:
  RegistryRef mRegistry;
  ComponentID mComponentId{};
  std::string mAttributeName;
  PropertyValue mUpdatedValue;
  Maybe<PropertyValue> mPreviousValue;
};

}  // namespace Tactile
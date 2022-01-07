#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include "core/property_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class UpdateComponentCmd final : public ACommand {
 public:
  UpdateComponentCmd(RegistryRef registry,
                     ContextID contextId,
                     ComponentID componentId,
                     std::string attribute,
                     PropertyValue value);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::UpdateComponent;
  }

 private:
  RegistryRef mRegistry;
  ContextID mContextId{};
  ComponentID mComponentId{};
  std::string mAttributeName;
  PropertyValue mUpdatedValue;
  Maybe<PropertyValue> mPreviousValue;
};

}  // namespace Tactile
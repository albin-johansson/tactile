#pragma once

#include <string>  // string

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class UpdateComponentCmd final : public ACommand {
 public:
  UpdateComponentCmd(RegistryRef registry,
                     context_id contextId,
                     component_id componentId,
                     std::string attribute,
                     attribute_value value);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::UpdateComponent;
  }

 private:
  RegistryRef mRegistry;
  context_id mContextId{};
  component_id mComponentId{};
  std::string mAttributeName;
  attribute_value mUpdatedValue;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile
#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class UpdateComponentAttributeCmd final : public ACommand {
 public:
  UpdateComponentAttributeCmd(RegistryRef registry,
                              component_id id,
                              std::string attribute,
                              attribute_value value);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::UpdateComponentAttribute;
  }

 private:
  RegistryRef mRegistry;
  component_id mComponentId{};
  std::string mAttributeName;
  attribute_value mUpdatedValue;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile
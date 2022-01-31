#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class RenameComponentAttributeCmd final : public command_base {
 public:
  RenameComponentAttributeCmd(registry_ref registry,
                              component_id id,
                              std::string previousName,
                              std::string updatedName);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::rename_component_attribute;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId{};
  std::string mPreviousName;
  std::string mUpdatedName;
};

}  // namespace tactile
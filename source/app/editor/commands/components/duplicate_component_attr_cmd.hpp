#pragma once

#include <string>  // string

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class duplicate_component_attr_cmd final : public command_base
{
 public:
  duplicate_component_attr_cmd(registry_ref registry,
                               component_id componentId,
                               std::string attribute);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::duplicate_component_attribute;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId{};
  std::string mAttributeName;
  maybe<std::string> mDuplicatedName;
};

}  // namespace tactile
#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class remove_component_attr_cmd final : public command_base
{
 public:
  remove_component_attr_cmd(registry_ref registry,
                            component_id componentId,
                            std::string attribute);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::remove_component_attribute;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId{};
  std::string mAttributeName;
  maybe<attribute_value> mPreviousDefault;
};

}  // namespace tactile
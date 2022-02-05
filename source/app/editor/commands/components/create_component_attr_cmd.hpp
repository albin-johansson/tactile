#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class create_component_attr_cmd final : public command_base
{
 public:
  create_component_attr_cmd(registry_ref registry, component_id id, std::string name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::create_component_def_attribute;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId{};
  std::string mName;
};

}  // namespace tactile
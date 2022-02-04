#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class CreateComponentDefCmd final : public command_base {
 public:
  CreateComponentDefCmd(registry_ref registry, std::string name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::create_component_def;
  }

 private:
  registry_ref mRegistry;
  std::string mName;
  maybe<component_id> mComponentId;
};

}  // namespace tactile
#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class create_component_def_cmd final : public command_base
{
 public:
  create_component_def_cmd(registry_ref registry, std::string name);

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
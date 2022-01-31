#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RenameComponentCmd final : public command_base {
 public:
  RenameComponentCmd(registry_ref registry, component_id id, std::string name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::rename_component;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId{};
  std::string mUpdatedName;
  maybe<std::string> mPreviousName;
};

}  // namespace tactile
#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class RenameComponentCmd final : public ACommand {
 public:
  RenameComponentCmd(RegistryRef registry, ComponentID id, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RenameComponent;
  }

 private:
  RegistryRef mRegistry;
  ComponentID mComponentId{};
  std::string mUpdatedName;
  Maybe<std::string> mPreviousName;
};

}  // namespace tactile
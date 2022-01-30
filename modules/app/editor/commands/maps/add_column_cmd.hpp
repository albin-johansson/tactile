#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class AddColumnCmd final : public ACommand {
 public:
  explicit AddColumnCmd(RegistryRef registry);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddColumn;
  }

 private:
  RegistryRef mRegistry;
  usize mColumns{1};
};

}  // namespace tactile

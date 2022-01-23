#pragma once

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "map_command_cache.hpp"

namespace tactile {

class RemoveRowCmd final : public ACommand {
 public:
  explicit RemoveRowCmd(RegistryRef registry);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveRow;
  }

 private:
  RegistryRef mRegistry;
  MapCommandCache mCache;
  usize mRows{1};
};

}  // namespace tactile

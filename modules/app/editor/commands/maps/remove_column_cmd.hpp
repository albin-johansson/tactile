#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "map_command_cache.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RemoveColumnCmd final : public ACommand {
 public:
  explicit RemoveColumnCmd(RegistryRef registry);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveColumn;
  }

 private:
  RegistryRef mRegistry;
  MapCommandCache mCache;
  usize mColumns{1};
};

}  // namespace tactile

#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "map_command_cache.hpp"

namespace Tactile {

class RemoveColumnCmd final : public ACommand
{
 public:
  explicit RemoveColumnCmd(Ref<entt::registry> registry);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveColumn;
  }

 private:
  Ref<entt::registry> mRegistry;
  MapCommandCache mCache;
  usize mColumns{1};
};

}  // namespace Tactile

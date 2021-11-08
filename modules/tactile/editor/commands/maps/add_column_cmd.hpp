#pragma once

#include <tactile_def.hpp>

#include <entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class AddColumnCmd final : public ACommand
{
 public:
  explicit AddColumnCmd(Ref<entt::registry> registry);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddColumn;
  }

 private:
  Ref<entt::registry> mRegistry;
  usize mColumns{1};
};

}  // namespace Tactile

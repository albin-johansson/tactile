#pragma once

#include <entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

class AddRowCmd final : public ACommand
{
 public:
  explicit AddRowCmd(Ref<entt::registry> registry);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddRow;
  }

 private:
  Ref<entt::registry> mRegistry;
  usize mRows{1};
};

}  // namespace Tactile

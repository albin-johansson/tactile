#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "map_command_cache.hpp"

namespace Tactile {

class ResizeMapCmd final : public ACommand
{
 public:
  ResizeMapCmd(Ref<entt::registry> registry, usize nRows, usize nCols);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::ResizeMap;
  }

 private:
  Ref<entt::registry> mRegistry;
  usize mRows{};
  usize mCols{};
  MapCommandCache mCache;
  Maybe<usize> mPrevRows{};
  Maybe<usize> mPrevCols{};

  [[nodiscard]] auto IsLossyResize() const -> bool;
};

}  // namespace Tactile

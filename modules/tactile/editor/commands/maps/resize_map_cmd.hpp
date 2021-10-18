#pragma once

#include <entt.hpp>  // registry

#include <tactile-base/tactile_std.hpp>

#include "map_command_cache.hpp"
#include "tactile/editor/commands/command.hpp"
#include "tactile/editor/commands/command_id.hpp"

namespace Tactile {

class ResizeMapCmd final : public ACommand
{
 public:
  ResizeMapCmd(Ref<entt::registry> registry, int nRows, int nCols);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::ResizeMap;
  }

 private:
  Ref<entt::registry> mRegistry;
  int mRows{};
  int mCols{};
  MapCommandCache mCache;
  Maybe<int> mPrevRows{};
  Maybe<int> mPrevCols{};

  [[nodiscard]] auto IsLossyResize() const -> bool;
};

}  // namespace Tactile

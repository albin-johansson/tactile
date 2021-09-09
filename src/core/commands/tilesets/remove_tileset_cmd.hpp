#pragma once

#include <entt.hpp>  // registry

#include "aliases/maybe.hpp"
#include "aliases/ref.hpp"
#include "aliases/tileset_id.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/systems/tileset_snapshot.hpp"

namespace Tactile {

class RemoveTilesetCmd final : public ACommand
{
 public:
  RemoveTilesetCmd(Ref<entt::registry> registry, TilesetID id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveTileset;
  }

 private:
  Ref<entt::registry> mRegistry;
  TilesetID mTilesetId;
  Maybe<Sys::TilesetSnapshot> mSnapshot;
};

}  // namespace Tactile

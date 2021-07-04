#include "mergeable_map_command.hpp"

#include <utility>  // move

namespace Tactile {

MergeableMapCommand::MergeableMapCommand(NotNull<MapDocument*> document,
                                         std::string text)
    : MapCommand{document, std::move(text)}
{}

auto MergeableMapCommand::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId())
  {
    auto& other = dynamic_cast<const MergeableMapCommand&>(cmd);
    mAmount += other.mAmount;

    for (const auto& [otherLayer, otherTileCache] : other.GetLayerCache())
    {
      auto& tileCache = GetTileCache(otherLayer);

      for (const auto& [otherPos, otherTile] : otherTileCache)
      {
        tileCache.emplace(otherPos, otherTile);
      }
    }

    return true;
  }

  return false;
}

}  // namespace Tactile

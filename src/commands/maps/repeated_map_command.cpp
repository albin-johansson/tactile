#include "repeated_map_command.hpp"

namespace tactile::cmd {

RepeatedMapCommand::RepeatedMapCommand(not_null<core::MapDocument*> document,
                                       const QString& name)
    : MapCommand{document, name}
{}

auto RepeatedMapCommand::mergeWith(const QUndoCommand* other) -> bool
{
  if (id() == other->id())
  {
    if (auto* ptr = dynamic_cast<const RepeatedMapCommand*>(other))
    {
      mTimes += ptr->mTimes;

      for (const auto& [layer, data] : ptr->LayerData())
      {
        auto& tiles = TileData(layer);
        for (const auto& [pos, tile] : data)
        {
          tiles.emplace(pos, tile);
        }
      }

      return true;
    }
  }
  return false;
}

}  // namespace tactile::cmd

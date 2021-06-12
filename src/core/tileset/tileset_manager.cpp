#include "tileset_manager.hpp"

#include <algorithm>  // any_of
#include <utility>    // move

#include "tactile_error.hpp"

namespace tactile::core {

TilesetManager::TilesetManager()
{
  mTilesets.reserve(5);
}

void TilesetManager::Add(const tileset_id id, shared<Tileset> tileset)
{
  Q_ASSERT(!Contains(id));
  Q_ASSERT(tileset);

  mNextGlobalTileId = tileset->LastId() + 1_t;
  mTilesets.emplace(id, std::move(tileset));
  mActiveId = id;
}

auto TilesetManager::Add(shared<Tileset> tileset) -> tileset_id
{
  Q_ASSERT(tileset);

  const auto id = mNextId;
  ++mNextId;

  Add(id, std::move(tileset));
  return id;
}

void TilesetManager::Remove(const tileset_id id) noexcept
{
  // TODO consider decrementing m_nextID and decreasing m_nextGlobalTileID
  mTilesets.erase(id);

  if (id == mActiveId)
  {
    mActiveId = nothing;
  }
}

void TilesetManager::Clear() noexcept
{
  mTilesets.clear();
}

void TilesetManager::Rename(const tileset_id id, const QString& name)
{
  mTilesets.at(id)->SetName(name);
}

void TilesetManager::Select(const Maybe<tileset_id> id)
{
  if (id)
  {
    Q_ASSERT(mTilesets.contains(*id));
    mActiveId = id;
  }
  else
  {
    mActiveId = nothing;
  }
}

void TilesetManager::SetSelection(const TilesetSelection& selection)
{
  if (HasActiveTileset())
  {
    mTilesets.at(*mActiveId)->SetSelection(selection);
  }
}

void TilesetManager::IncrementNextTilesetId() noexcept
{
  ++mNextId;
}

auto TilesetManager::At(const tileset_id id) -> Tileset&
{
  return *mTilesets.at(id);
}

auto TilesetManager::At(const tileset_id id) const -> const Tileset&
{
  return *mTilesets.at(id);
}

auto TilesetManager::Image(const tile_id id) const -> const QPixmap&
{
  for (const auto& [key, tileset] : mTilesets)
  {
    if (tileset->Contains(id))
    {
      return tileset->Image();
    }
  }
  throw TactileError{"tileset_manager > failed to find image for tile ID!"};
}

auto TilesetManager::ImageSource(tile_id id) const -> QRect
{
  for (const auto& [key, tileset] : mTilesets)
  {
    if (const auto rect = tileset->ImageSource(id))
    {
      return *rect;
    }
  }
  throw TactileError{"tileset_manager > failed to obtain source rectangle!"};
}

auto TilesetManager::RangeOf(tileset_id id) const -> tile_range
{
  Q_ASSERT(Contains(id));

  const auto& tileset = mTilesets.at(id);
  return {tileset->FirstId(), tileset->LastId()};
}

auto TilesetManager::Count() const noexcept -> int
{
  return static_cast<int>(mTilesets.size());
}

auto TilesetManager::HasActiveTileset() const noexcept -> bool
{
  return mActiveId.has_value();
}

auto TilesetManager::CurrentTileset() const -> const Tileset*
{
  return mActiveId ? mTilesets.at(*mActiveId).get() : nullptr;
}

auto TilesetManager::CurrentTilesetId() const -> Maybe<tileset_id>
{
  return mActiveId;
}

auto TilesetManager::Contains(const tile_id id) const -> bool
{
  return std::ranges::any_of(mTilesets, [id](const auto& pair) {
    return pair.second->Contains(id);
  });
}

auto TilesetManager::Contains(const tileset_id id) const -> bool
{
  return mTilesets.contains(id);
}

}  // namespace tactile::core

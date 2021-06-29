#include "tileset_manager.hpp"

#include <algorithm>  // any_of
#include <cassert>    // assert
#include <utility>    // move

#include "core/tactile_error.hpp"

namespace Tactile {

TilesetManager::TilesetManager()
{
  mTilesets.reserve(8);
}

void TilesetManager::Add(const tileset_id id, Shared<Tileset> tileset)
{
  assert(!Contains(id));
  assert(tileset);

  mNextGlobalTileId = tileset->GetLastId() + 1_tile;

  mCurrentTileset = id;
  mTilesets.emplace(id, std::move(tileset));

  if (id == mNextId)
  {
    ValidateNextTilesetId();
  }
}

auto TilesetManager::Add(Shared<Tileset> tileset) -> tileset_id
{
  assert(tileset);

  // Avoid clashing IDs
  ValidateNextTilesetId();

  const auto id = mNextId;
  Add(id, std::move(tileset));

  return id;
}

void TilesetManager::Remove(const tileset_id id)
{
  assert(Contains(id));

  mTilesets.erase(id);
  if (id == mCurrentTileset)
  {
    mCurrentTileset = nothing;
  }
}

void TilesetManager::Clear()
{
  mTilesets.clear();
  mCurrentTileset = nothing;
}

void TilesetManager::Rename(const tileset_id id, std::string name)
{
  assert(Contains(id));

  auto& tileset = mTilesets.at(id);
  tileset->SetName(std::move(name));
}

void TilesetManager::Select(const Maybe<tileset_id> id)
{
  if (id)
  {
    assert(Contains(*id));
    mCurrentTileset = id;
  }
  else
  {
    mCurrentTileset = nothing;
  }
}

void TilesetManager::SetSelection(const TilesetSelection& selection)
{
  assert(HasActiveTileset());

  auto& tileset = mTilesets.at(mCurrentTileset.value());
  tileset->SetSelection(selection);
}

void TilesetManager::IncrementNextTilesetId()
{
  ++mNextId;
  ValidateNextTilesetId();
}

auto TilesetManager::GetRef(const tileset_id id) -> Tileset&
{
  return *mTilesets.at(id);
}

auto TilesetManager::GetRef(const tileset_id id) const -> const Tileset&
{
  return *mTilesets.at(id);
}

auto TilesetManager::GetPtr(const tileset_id id) -> Shared<Tileset>
{
  return mTilesets.at(id);
}

auto TilesetManager::GetName(const tileset_id id) -> std::string_view
{
  assert(Contains(id));

  const auto& tileset = mTilesets.at(id);
  return tileset->GetName();
}

auto TilesetManager::GetTexture(const tile_id id) const -> GLuint
{
  for (const auto& [key, tileset] : mTilesets)
  {
    if (tileset->Contains(id))
    {
      return tileset->GetTexture();
    }
  }

  throw TactileError{"Tileset manager could not find texture!"};
}

auto TilesetManager::GetImageSource(const tile_id id) const -> cen::irect
{
  for (const auto& [key, tileset] : mTilesets)
  {
    if (const auto rectangle = tileset->GetImageSource(id))
    {
      return *rectangle;
    }
  }

  throw TactileError{"Tileset manager could not find tile source rectangle!"};
}

auto TilesetManager::GetRange(const tileset_id id) const -> TileRange
{
  assert(Contains(id));

  const auto& tileset = mTilesets.at(id);
  return {tileset->GetFirstId(), tileset->GetLastId()};
}

auto TilesetManager::GetSize() const -> usize
{
  return mTilesets.size();
}

auto TilesetManager::HasActiveTileset() const -> bool
{
  return mCurrentTileset.has_value();
}

auto TilesetManager::GetActiveTileset() const -> const Tileset*
{
  return mCurrentTileset ? mTilesets.at(*mCurrentTileset).get() : nullptr;
}

auto TilesetManager::GetActiveTilesetId() const -> Maybe<tileset_id>
{
  return mCurrentTileset;
}

auto TilesetManager::Contains(const tile_id id) const -> bool
{
  using Pair = storage_type::value_type;
  return std::ranges::any_of(mTilesets, [id](const Pair& pair) {
    return pair.second->Contains(id);
  });
}

auto TilesetManager::Contains(const tileset_id id) const -> bool
{
  return mTilesets.contains(id);
}

void TilesetManager::ValidateNextTilesetId()
{
  while (Contains(mNextId))
  {
    ++mNextId;
  }
}

}  // namespace Tactile

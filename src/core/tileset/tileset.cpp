#include "tileset.hpp"

#include <utility>  // move

#include "core/tactile_error.hpp"

namespace Tactile {
namespace {

[[nodiscard]] auto CreateSourceRectCache(const tile_id first,
                                         const tile_id last,
                                         const col_t columnCount,
                                         const int tileWidth,
                                         const int tileHeight)
    -> std::unordered_map<tile_id, cen::irect>
{
  std::unordered_map<tile_id, cen::irect> cache;

  const auto amount = (last + 1_tile) - first;
  cache.reserve(amount.get());

  for (tile_id id{first}; id <= last; ++id)
  {
    const auto index = id - first;

    const auto x = (index.get() % columnCount.get()) * tileWidth;
    const auto y = (index.get() / columnCount.get()) * tileHeight;

    cache.emplace(id, cen::irect{x, y, tileWidth, tileHeight});
  }

  return cache;
}

}  // namespace

Tileset::Tileset(const tile_id firstId,
                 const TextureInfo& info,
                 const int tileWidth,
                 const int tileHeight)
    : mTexture{info.texture}
    , mWidth{info.width}
    , mHeight{info.height}
    , mTileWidth{tileWidth}
    , mTileHeight{tileHeight}
    , mFirstId{firstId}
    , mPath{info.path}
    , mProperties{std::make_unique<PropertyDelegate>()}
{
  if (mTileWidth < 1 || mTileHeight < 1)
  {
    throw TactileError{"Invalid tileset tile dimensions!"};
  }

  SetName(mPath.stem().string());

  mRowCount = AsRow(GetHeight() / mTileHeight);
  mColumnCount = AsColumn(GetWidth() / mTileWidth);

  mTileCount = mRowCount.get() * mColumnCount.get();
  mLastId = mFirstId + tile_id{mTileCount};

  mSourceRects = CreateSourceRectCache(mFirstId,
                                       mLastId,
                                       mColumnCount,
                                       mTileWidth,
                                       mTileHeight);
}

void Tileset::UpdateAnimations()
{
  for (auto& [tile, animation] : mAnimations)
  {
    animation.Update();
  }
}

void Tileset::SetSelection(const Region& selection)
{
  mSelection = selection;
}

void Tileset::ClearSelection() noexcept
{
  mSelection.reset();
}

void Tileset::SetName(std::string name)
{
  mProperties->SetName(std::move(name));
}

void Tileset::SetPath(std::filesystem::path path)
{
  mPath = std::move(path);
}

void Tileset::SetAnimation(const tile_id id, const TileAnimation& animation)
{
  mAnimations.try_emplace(id, animation);
}

auto Tileset::Contains(const tile_id id) const -> bool
{
  return (id >= GetFirstId()) && (id <= GetLastId());
}

auto Tileset::IsSingleTileSelected() const -> bool
{
  return mSelection &&
         (mSelection->end - mSelection->begin == MapPosition{1_row, 1_col});
}

auto Tileset::IsAnimated(const tile_id id) const -> bool
{
  return mAnimations.contains(id);
}

auto Tileset::GetAnimatedTile(const tile_id id) const -> tile_id
{
  return mAnimations.at(id).GetCurrentTile();
}

auto Tileset::GetTile(const MapPosition& position) const -> tile_id
{
  const auto row = position.GetRow().get();
  const auto col = position.GetColumn().get();
  const auto endRow = GetRowCount();
  const auto endCol = GetColumnCount();

  if ((row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol))
  {
    const auto index = row * mColumnCount.get() + col;
    return mFirstId + tile_id{index};
  }
  else
  {
    return empty_tile;
  }
}

auto Tileset::GetImageSource(const tile_id id) const -> Maybe<cen::irect>
{
  if (const auto it = mSourceRects.find(id); it != mSourceRects.end())
  {
    return it->second;
  }
  else
  {
    return nothing;
  }
}

void Tileset::AddProperty(std::string name, const PropertyType type)
{
  mProperties->AddProperty(std::move(name), type);
}

void Tileset::AddProperty(std::string name, const Property& property)
{
  mProperties->AddProperty(std::move(name), property);
}

void Tileset::RemoveProperty(const std::string_view name)
{
  mProperties->RemoveProperty(name);
}

void Tileset::RenameProperty(const std::string_view oldName, std::string newName)
{
  mProperties->RenameProperty(oldName, std::move(newName));
}

void Tileset::SetProperty(const std::string_view name, const Property& property)
{
  mProperties->SetProperty(name, property);
}

void Tileset::ChangePropertyType(std::string name, const PropertyType type)
{
  mProperties->ChangePropertyType(std::move(name), type);
}

auto Tileset::HasProperty(const std::string_view name) const -> bool
{
  return mProperties->HasProperty(name);
}

auto Tileset::GetProperty(const std::string_view name) const -> const Property&
{
  return mProperties->GetProperty(name);
}

auto Tileset::GetProperties() const -> const PropertyMap&
{
  return mProperties->GetProperties();
}

auto Tileset::GetPropertyCount() const -> usize
{
  return mProperties->GetPropertyCount();
}

auto Tileset::GetName() const -> const std::string&
{
  return mProperties->GetName();
}

}  // namespace Tactile

#include "tileset_tab_manager.hpp"

#include "tileset_tab.hpp"

namespace tactile {

void TilesetTabManager::Add(tileset_id id, TilesetTab* tab)
{
  Q_ASSERT(tab);
  Q_ASSERT(tab->parent());  // require that the tab has an owner
  mTabs.emplace(id, tab);
}

void TilesetTabManager::Remove(tileset_id id)
{
  mTabs.erase(id);
}

void TilesetTabManager::Clear() noexcept
{
  mTabs.clear();
}

void TilesetTabManager::SetCachedIndex(const int index)
{
  mCachedIndex = index;
}

auto TilesetTabManager::At(const tileset_id id) const -> const TilesetTab*
{
  return mTabs.at(id);
}

auto TilesetTabManager::IndexOf(const tileset_id id) const -> int
{
  return static_cast<int>(mTabs.index_of(id).value());
}

auto TilesetTabManager::Contains(const tileset_id id) const -> bool
{
  return mTabs.contains(id);
}

auto TilesetTabManager::IsEmpty() const -> bool
{
  return mTabs.empty();
}

}  // namespace tactile

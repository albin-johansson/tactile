#pragma once

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "tileset_id.hpp"
#include "vector_map.hpp"

TACTILE_FORWARD_DECLARE(tactile, TilesetTab)

namespace tactile {

class TilesetTabManager final
{
 public:
  void Add(tileset_id id, TilesetTab* tab);

  void Remove(tileset_id id);

  void Clear() noexcept;

  void SetCachedIndex(int index);

  [[nodiscard]] auto At(tileset_id id) const -> const TilesetTab*;

  [[nodiscard]] auto IndexOf(tileset_id id) const -> int;

  [[nodiscard]] auto Contains(tileset_id id) const -> bool;

  [[nodiscard]] auto IsEmpty() const -> bool;

  [[nodiscard]] auto CachedIndex() const -> Maybe<int>
  {
    return mCachedIndex;
  }

  [[nodiscard]] auto begin() noexcept
  {
    return mTabs.begin();
  }

  [[nodiscard]] auto begin() const noexcept
  {
    return mTabs.begin();
  }

  [[nodiscard]] auto end() noexcept
  {
    return mTabs.end();
  }

  [[nodiscard]] auto end() const noexcept
  {
    return mTabs.end();
  }

 private:
  vector_map<tileset_id, TilesetTab*> mTabs;
  Maybe<int> mCachedIndex;
};

}  // namespace tactile

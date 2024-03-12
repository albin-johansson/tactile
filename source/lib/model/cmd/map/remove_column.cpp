// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "remove_column.hpp"

#include <utility>  // move

#include "common/util/functional.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RemoveColumn::RemoveColumn(Shared<Map> map)
    : mMap {std::move(map)}
{
  if (!mMap) {
    throw Exception {"Invalid null map"};
  }
}

void RemoveColumn::undo()
{
  invoke_n(mColumnCount, [this] { mMap->add_column(); });
  mCache.restore_tiles(*mMap);
}

void RemoveColumn::redo()
{
  const auto map_extent = mMap->get_extent();

  const auto begin = TilePos::from(0u, map_extent.cols - mColumnCount - 1u);
  const auto end = TilePos::from(map_extent.rows, map_extent.cols);

  mCache.clear();
  mCache.save_tiles(*mMap, begin, end);

  invoke_n(mColumnCount, [this] { mMap->remove_column(); });
}

auto RemoveColumn::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RemoveColumn*>(cmd)) {
    mColumnCount += other->mColumnCount;
    mCache.merge_with(other->mCache);
    return true;
  }

  return false;
}

auto RemoveColumn::get_name() const -> String
{
  const auto& lang = get_current_language();
  return mColumnCount == 1 ? lang.cmd.remove_column : lang.cmd.remove_columns;
}

}  // namespace tactile::cmd

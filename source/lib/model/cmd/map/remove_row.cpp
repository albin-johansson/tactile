// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "remove_row.hpp"

#include <utility>  // move

#include "common/util/functional.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RemoveRow::RemoveRow(Shared<Map> map)
    : mMap {std::move(map)}
{
  if (!mMap) {
    throw Exception {"Invalid null map"};
  }
}

void RemoveRow::undo()
{
  invoke_n(mRowCount, [this] { mMap->add_row(); });
  mCache.restore_tiles(*mMap);
}

void RemoveRow::redo()
{
  const auto map_extent = mMap->get_extent();

  const auto begin = TilePos::from(map_extent.rows - mRowCount - 1u, 0u);
  const auto end = TilePos::from(map_extent.rows, map_extent.cols);

  mCache.clear();
  mCache.save_tiles(*mMap, begin, end);

  invoke_n(mRowCount, [this] { mMap->remove_row(); });
}

auto RemoveRow::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RemoveRow*>(cmd)) {
    mRowCount += other->mRowCount;
    mCache.merge_with(other->mCache);
    return true;
  }

  return false;
}

auto RemoveRow::get_name() const -> String
{
  const auto& lang = get_current_language();
  return mRowCount == 1 ? lang.cmd.remove_row : lang.cmd.remove_rows;
}

}  // namespace tactile::cmd

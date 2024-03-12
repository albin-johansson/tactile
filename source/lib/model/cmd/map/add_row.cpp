// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "add_row.hpp"

#include <utility>  // move

#include "common/util/functional.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

AddRow::AddRow(Shared<Map> map)
    : mMap {std::move(map)}
{
  if (!mMap) {
    throw Exception {"Invalid null map"};
  }
}

void AddRow::undo()
{
  invoke_n(mRowCount, [this] { mMap->remove_row(); });
}

void AddRow::redo()
{
  invoke_n(mRowCount, [this] { mMap->add_row(); });
}

auto AddRow::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const AddRow*>(cmd)) {
    mRowCount += other->mRowCount;
    return true;
  }

  return false;
}

auto AddRow::get_name() const -> String
{
  const auto& lang = get_current_language();
  return mRowCount == 1 ? lang.cmd.add_row : lang.cmd.add_rows;
}

}  // namespace tactile::cmd

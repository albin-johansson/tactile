// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "add_column.hpp"

#include <utility>  // move

#include "common/util/functional.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

AddColumn::AddColumn(Shared<Map> map)
    : mMap {std::move(map)}
{
  if (!mMap) {
    throw Exception {"Invalid null map"};
  }
}

void AddColumn::undo()
{
  invoke_n(mColumnCount, [this] { mMap->remove_column(); });
}

void AddColumn::redo()
{
  invoke_n(mColumnCount, [this] { mMap->add_column(); });
}

auto AddColumn::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const AddColumn*>(cmd)) {
    mColumnCount += other->mColumnCount;
    return true;
  }

  return false;
}

auto AddColumn::get_name() const -> String
{
  const auto& lang = get_current_language();
  return mColumnCount == 1 ? lang.cmd.add_column : lang.cmd.add_columns;
}

}  // namespace tactile::cmd

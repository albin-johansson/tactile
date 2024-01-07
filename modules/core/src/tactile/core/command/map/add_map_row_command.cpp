// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/map/add_map_row_command.hpp"

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/foundation/misc/tile_matrix.hpp"

namespace tactile {

AddMapRowCommand::AddMapRowCommand(MapDocument* document)
  : mDocument {require_not_null(document, "null document")}
{}

void AddMapRowCommand::undo()
{
  TACTILE_LOG_TRACE("[AddMapRowCommand::undo]");

  auto& map = mDocument->map();

  const auto old_extent = map.extent();
  const auto new_extent =
      MatrixExtent {old_extent.row_count - mRowCount, old_extent.col_count};

  map.set_extent(new_extent);
}

void AddMapRowCommand::redo()
{
  TACTILE_LOG_TRACE("[AddMapRowCommand::redo]");

  auto& map = mDocument->map();

  const auto old_extent = map.extent();
  const auto new_extent =
      MatrixExtent {old_extent.row_count + mRowCount, old_extent.col_count};

  map.set_extent(new_extent);
}

auto AddMapRowCommand::merge_with(const ICommand* cmd) -> bool
{
  if (const auto* that = dynamic_cast<const AddMapRowCommand*>(cmd)) {
    if (this->mDocument->map().meta().uuid() == that->mDocument->map().meta().uuid()) {
      mRowCount += that->mRowCount;
      return true;
    }
  }

  return false;
}

}  // namespace tactile

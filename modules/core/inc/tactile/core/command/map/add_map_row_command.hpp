// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class MapDocument;

/**
 * Command for adding tile rows to a map.
 */
class TACTILE_CORE_API AddMapRowCommand final : public ICommand {
 public:
  /**
   * Creates a command.
   *
   * \param document the associated map document, cannot be null.
   */
  explicit AddMapRowCommand(MapDocument* document);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* cmd) -> bool override;

 private:
  MapDocument* mDocument;
  ssize mRowCount {1};
};

}  // namespace tactile::core

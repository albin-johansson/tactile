// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class MapDocument;

/**
 * A command for changing the visibility of layers.
 */
class TACTILE_CORE_API UpdateLayerVisibilityCommand final : public ICommand {
 public:
  /**
   * Creates a new command.
   *
   * \param document   The host document, cannot be null.
   * \param layer_uuid The target layer UUID.
   * \param visibility The new layer visibility.
   */
  UpdateLayerVisibilityCommand(MapDocument* document,
                               const UUID& layer_uuid,
                               bool visibility);

  void undo() override;

  void redo() override;

 private:
  MapDocument* mDocument;
  UUID mLayerUuid;
  bool mNewVisibility;
  Maybe<bool> mOldVisibility;
};

}  // namespace tactile::core

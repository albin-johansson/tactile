// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class MapDocument;
class ILayer;

/**
 * A command for removing layers from maps.
 */
class TACTILE_CORE_API RemoveLayerCommand final : public ICommand {
 public:
  /**
   * Creates a command.
   *
   * \param document   The host document, cannot be null.
   * \param layer_uuid The target layer UUID.
   */
  RemoveLayerCommand(MapDocument* document, const UUID& layer_uuid);

  void undo() override;

  void redo() override;

 private:
  MapDocument* mDocument;
  UUID mLayerUuid;
  Maybe<UUID> mRemovedLayerParentUuid;
  Maybe<ssize> mRemovedLayerLocalIndex;
  Shared<ILayer> mRemovedLayer;
};

}  // namespace tactile::core

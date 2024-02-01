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
 * A command for changing the opacity of layers.
 */
class TACTILE_CORE_API UpdateLayerOpacityCommand final : public ICommand {
 public:
  /**
   * Creates a new command.
   *
   * \param document   The host document, cannot be null.
   * \param layer_uuid The target layer UUID.
   * \param opacity    The new layer opacity.
   */
  UpdateLayerOpacityCommand(MapDocument* document, const UUID& layer_uuid, float opacity);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* other) -> bool override;

 private:
  MapDocument* mDocument;
  UUID mLayerUuid;
  float mNewOpacity;
  Maybe<float> mOldOpacity;
};

}  // namespace tactile::core

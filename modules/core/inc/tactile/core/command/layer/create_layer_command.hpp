// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/misc/layer_type.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class MapDocument;
class ILayer;

/**
 * A command for creating new layers.
 */
class TACTILE_CORE_API CreateLayerCommand final : public ICommand {
 public:
  /**
   * Creates a new command.
   *
   * \param document The host document, cannot be null.
   * \param type     The type of the new layer.
   */
  CreateLayerCommand(MapDocument* document, LayerType type);

  void undo() override;

  void redo() override;

 private:
  MapDocument* mDocument;
  LayerType mLayerType;
  Maybe<UUID> mNewLayerUuid;
  Maybe<UUID> mNewLayerParentUuid;

  [[nodiscard]]
  auto _create_layer() -> Shared<ILayer>;
};

}  // namespace tactile::core

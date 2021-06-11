#pragma once

#include <QStandardItem>

#include "layer.hpp"
#include "layer_id.hpp"
#include "layer_type.hpp"

namespace tactile::vm {

class LayerItem final : public QStandardItem
{
 public:
  LayerItem(layer_id id, const core::ILayer& layer);

  void SetId(layer_id id);

  void SetType(core::LayerType type);

  [[nodiscard]] auto GetId() const -> layer_id;

  [[nodiscard]] auto GetLayerType() const -> core::LayerType;
};

}  // namespace tactile::vm

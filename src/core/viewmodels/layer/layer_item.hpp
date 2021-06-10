#pragma once

#include <QStandardItem>

#include "layer.hpp"
#include "layer_id.hpp"
#include "layer_type.hpp"

namespace tactile::vm {

class layer_item final : public QStandardItem
{
 public:
  explicit layer_item(const QString& name);

  [[nodiscard]] static auto make(layer_id id, const core::ILayer& layer)
      -> layer_item*;

  void set_id(layer_id id);

  void set_type(core::layer_type type);

  [[nodiscard]] auto get_id() const -> layer_id;

  [[nodiscard]] auto get_layer_type() const -> core::layer_type;
};

}  // namespace tactile::vm

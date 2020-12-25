#pragma once

#include <QStandardItemModel>

#include "map_document.hpp"

namespace tactile::vm {

class layer_model final : public QStandardItemModel
{
  Q_OBJECT

 public:
  explicit layer_model(core::map_document* document);

  void add_tile_layer();

  void add_object_layer();

 private:
  core::map_document* m_document{};

  void add_item(layer_id id, const core::layer& layer);

  void remove_item(layer_id id);
};

}  // namespace tactile::vm

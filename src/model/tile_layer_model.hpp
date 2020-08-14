#pragma once

#include <QObject>

#include "tile_layer.hpp"

namespace tactile::model {

class tile_layer_model final : public QObject {
  Q_OBJECT

 public:
  explicit tile_layer_model(QObject* parent = nullptr);

 signals:
  void layer_added(tile_layer& layer);

  void layer_removed(tile_layer& layer);

 private:



};

}  // namespace tactile::model

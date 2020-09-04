#pragma once

#include <qobject.h>

#include "layer.hpp"

namespace tactile::model {

class layer_model final : public QObject
{
  Q_OBJECT

 public:
  explicit layer_model(QObject* parent = nullptr);

 signals:
  void layer_added(layer& layer);

  void layer_removed(layer& layer);

 private:
};

}  // namespace tactile::model

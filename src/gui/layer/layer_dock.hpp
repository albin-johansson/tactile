#pragma once

#include "dock_widget.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)
TACTILE_FORWARD_DECLARE(tactile, LayerWidget)

namespace tactile {

class LayerDock final : public DockWidget
{
  Q_OBJECT

 public:
  explicit LayerDock(QWidget* parent = nullptr);

 public slots:
  void OnSwitchedMap(not_null<core::MapDocument*> document);

 private:
  LayerWidget* mWidget{};
};

}  // namespace tactile

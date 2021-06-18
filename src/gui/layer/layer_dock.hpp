#pragma once

#include "dock_widget.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile {

class LayerWidget;

class LayerDock final : public DockWidget
{
  Q_OBJECT

 public:
  explicit LayerDock(QWidget* parent = nullptr);

 public slots:
  void OnSwitchedMap(NotNull<core::MapDocument*> document);

 private:
  LayerWidget* mWidget{};
};

}  // namespace tactile

#pragma once

#include "dock_widget.hpp"
#include "fwd.hpp"
#include "not_null.hpp"
#include "tactile_declare_ui.hpp"

namespace tactile {

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

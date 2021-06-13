#include "layer_dock.hpp"

#include "layer_widget.hpp"

namespace tactile {

LayerDock::LayerDock(QWidget* parent)
    : DockWidget{parent}
    , mWidget{new LayerWidget{this}}
{
  setObjectName(QStringLiteral(u"LayerDock"));
  setWindowTitle(tr("Layers"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(mWidget);
}

void LayerDock::OnSwitchedMap(not_null<core::MapDocument*> document)
{
  mWidget->OnSwitchedMap(document);
}

}  // namespace tactile

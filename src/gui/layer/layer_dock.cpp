#include "layer_dock.hpp"

#include "layer_widget.hpp"
#include "tactile_qstring.hpp"

namespace tactile {

LayerDock::LayerDock(QWidget* parent)
    : DockWidget{parent}
    , mWidget{new LayerWidget{this}}
{
  setObjectName(TACTILE_QSTRING(u"layer_dock"));
  setWindowTitle(tr("Layers"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(mWidget);
}

void LayerDock::OnSwitchedMap(not_null<core::map_document*> document)
{
  mWidget->OnSwitchedMap(document);
}

}  // namespace tactile

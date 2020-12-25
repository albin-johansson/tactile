#include "properties_dock.hpp"

#include "properties_widget.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

properties_dock::properties_dock(QWidget* parent)
    : dock_widget{parent}
    , m_widget{new properties_widget{this}}
{
  setObjectName(TACTILE_QSTRING(u"properties_dock"));
  setWindowTitle(tr("Properties"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(m_widget);
}

void properties_dock::selected_map(
    const core::map_document& document,
    const shared<vm::property_model>& propertyModel)
{
  m_widget->selected_map(document, propertyModel);
}

}  // namespace tactile::gui

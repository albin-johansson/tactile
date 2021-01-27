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

void properties_dock::switched_map(const core::map_document& document)
{
  m_widget->selected_map(document);
}

void properties_dock::added_property(const QString& name)
{
  m_widget->added_property(name);
}

void properties_dock::about_to_remove_property(const QString& name)
{
  m_widget->about_to_remove_property(name);
}

void properties_dock::updated_property(const QString& name)
{
  m_widget->updated_property(name);
}

void properties_dock::renamed_property(const QString& oldName,
                                       const QString& newName)
{
  m_widget->renamed_property(oldName, newName);
}

}  // namespace tactile::gui

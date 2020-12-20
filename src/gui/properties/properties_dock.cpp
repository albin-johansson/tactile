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

  // clang-format off
  using widget = properties_widget;
  using dock = properties_dock;
  connect(m_widget, &widget::request_add_property, this, &dock::request_add_property);
  connect(m_widget, &widget::request_remove_property, this, &dock::request_remove_property);
  connect(m_widget, &widget::has_renamed_property, this, &dock::has_renamed_property);
  connect(m_widget, &widget::has_set_property, this, &dock::has_set_property);
  // clang-format on
}

void properties_dock::selected_map(const core::map_document& document)
{
  m_widget->selected_map(document);
}

void properties_dock::added_property(const QString& name,
                                     const core::property& property)
{
  m_widget->added_property(name, property);
}

void properties_dock::removed_property(const QString& name)
{
  m_widget->removed_property(name);
}

void properties_dock::moved_property_up(const QString& name)
{
  m_widget->moved_property_up(name);
}

void properties_dock::moved_property_down(const QString& name)
{
  m_widget->moved_property_down(name);
}

void properties_dock::duplicated_property(const QString& name)
{
  m_widget->duplicated_property(name);
}

}  // namespace tactile::gui

#include "property_object_item.hpp"

#include "tactile_qstring.hpp"

namespace tactile::gui {

property_object_item::property_object_item(const QString& name,
                                           const core::property& property,
                                           QTreeWidgetItem* parent)
    : property_tree_item{parent}
{
  Q_ASSERT(parent);
  Q_ASSERT(property.is<core::object_ref>());

  setText(0, name);
  setToolTip(0, TACTILE_QSTRING(u"(object)"));

//  Q_ASSERT(treeWidget());
//  treeWidget()->setItemWidget(this, 1, preview);
}

void property_object_item::set_value(const core::property& property)
{}

auto property_object_item::property_type() const noexcept
    -> core::property::type
{
  return core::property::object;
}

auto property_object_item::get_value_widget() -> QWidget*
{
  return nullptr;
}

}  // namespace tactile::gui

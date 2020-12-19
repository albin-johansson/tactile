#include "property_bool_item.hpp"

#include "tactile_qstring.hpp"

namespace tactile::gui {

property_bool_item::property_bool_item(const QString& name,
                                       const core::property& property,
                                       QTreeWidgetItem* parent)
    : property_tree_item{parent}
{
  Q_ASSERT(parent);
  setText(0, name);
  setToolTip(0, TACTILE_QSTRING(u"(bool)"));

  Q_ASSERT(treeWidget());
  treeWidget()->setItemWidget(this, 1, new QCheckBox{});

  set_value(property);
}

void property_bool_item::set_value(const core::property& property)
{
  Q_ASSERT(property.is_boolean());
  if (auto* widget = get_value_widget()) {
    widget->setCheckState(property.as<bool>() ? Qt::Checked : Qt::Unchecked);
  }
}

auto property_bool_item::property_type() const noexcept -> core::property::type
{
  return core::property::boolean;
}

auto property_bool_item::is_inline() const noexcept -> bool
{
  return false;
}

auto property_bool_item::get_value_widget() -> QCheckBox*
{
  Q_ASSERT(treeWidget());
  return qobject_cast<QCheckBox*>(treeWidget()->itemWidget(this, 1));
}

}  // namespace tactile::gui

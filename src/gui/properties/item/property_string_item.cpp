#include "property_string_item.hpp"

#include <QValidator>

#include "tactile_qstring.hpp"

namespace tactile::gui {
namespace {

class string_validator final : public QValidator
{
 public:
  [[nodiscard]] auto validate(QString& string, int&) const -> State override
  {
    if (string.isNull()) {
      return Invalid;
    } else if (string.isEmpty()) {
      return Intermediate;
    } else {
      return Acceptable;
    }
  }
};

}  // namespace

property_string_item::property_string_item(const QString& name,
                                           const core::property& property,
                                           QTreeWidgetItem* parent)
    : property_tree_item{parent}
{
  Q_ASSERT(parent);
  setText(0, name);
  setToolTip(0, TACTILE_QSTRING(u"(string)"));

  auto* edit = new QLineEdit{};
  edit->setFrame(false);
  edit->setValidator(new string_validator{});
  edit->setAutoFillBackground(true);

  Q_ASSERT(treeWidget());
  treeWidget()->setItemWidget(this, 1, edit);

  set_value(property);
}

void property_string_item::set_value(const core::property& property)
{
  Q_ASSERT(property.is_string());
  if (auto* widget = get_value_widget()) {
    widget->setText(property.as<QString>());
  }
}

auto property_string_item::property_type() const noexcept
    -> core::property::type
{
  return core::property::string;
}

auto property_string_item::get_value_widget() -> QLineEdit*
{
  Q_ASSERT(treeWidget());
  return qobject_cast<QLineEdit*>(treeWidget()->itemWidget(this, 1));
}

}  // namespace tactile::gui

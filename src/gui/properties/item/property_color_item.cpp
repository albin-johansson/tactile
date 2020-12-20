#include "property_color_item.hpp"

#include "color_preview_button.hpp"
#include "color_validator.hpp"
#include "property_int_item.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

property_color_item::property_color_item(const QString& name,
                                         const core::property& property,
                                         QTreeWidgetItem* parent)
    : property_tree_item{name, parent}
{
  Q_ASSERT(property.is<QColor>());
  setToolTip(0, TACTILE_QSTRING(u"(color)"));

  auto* preview = new color_preview_button{Qt::black};

  Q_ASSERT(treeWidget());
  treeWidget()->setItemWidget(this, 1, preview);

  Q_ASSERT(preview->parent());
  setup_color_preview(preview);
}

void property_color_item::setup_color_preview(color_preview_button* preview)
{
  const auto addColor = [this](const QString& name, const int value) {
    auto* item = new property_int_item{name, core::property{value}, this};
    auto* edit = item->get_value_widget();
    edit->setValidator(new color_validator{});
    return edit;
  };

  auto* redEdit = addColor(TACTILE_QSTRING(u"Red"), 0);
  auto* greenEdit = addColor(TACTILE_QSTRING(u"Green"), 0);
  auto* blueEdit = addColor(TACTILE_QSTRING(u"Blue"), 0);
  auto* alphaEdit = addColor(TACTILE_QSTRING(u"Alpha"), 255);

  QObject::connect(preview,
                   &color_preview_button::color_changed,
                   [=](const QColor& color) {
                     redEdit->setText(QString::number(color.red()));
                     greenEdit->setText(QString::number(color.green()));
                     blueEdit->setText(QString::number(color.blue()));
                     alphaEdit->setText(QString::number(color.alpha()));
                   });

  connect_color_line_edit(redEdit, [preview](const int value) {
    preview->set_red(value);
  });

  connect_color_line_edit(greenEdit, [preview](const int value) {
    preview->set_green(value);
  });

  connect_color_line_edit(blueEdit, [preview](const int value) {
    preview->set_blue(value);
  });

  connect_color_line_edit(alphaEdit, [preview](const int value) {
    preview->set_alpha(value);
  });
}

void property_color_item::set_value(const core::property& property)
{
  Q_ASSERT(property.is<QColor>());
  if (auto* button = qobject_cast<color_preview_button*>(
          treeWidget()->itemWidget(this, 1))) {
    button->set_color(property.as<QColor>());
  }
}

auto property_color_item::property_type() const noexcept -> core::property::type
{
  return core::property::color;
}

auto property_color_item::get_value_widget() -> QWidget*
{
  return nullptr;
}

}  // namespace tactile::gui

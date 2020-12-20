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
    } else {
      return Acceptable;
    }
  }
};

}  // namespace

string_value_widget::string_value_widget(QWidget* parent) : QLineEdit{parent}
{
  setFrame(false);

  auto* validator = new string_validator{};
  validator->setParent(this);

  setValidator(validator);
  setAutoFillBackground(true);
}

property_string_item::property_string_item(const QString& name,
                                           const core::property& property,
                                           QTreeWidgetItem* parent)
    : property_tree_item{parent}
    , m_widget{new string_value_widget{}}
{
  Q_ASSERT(parent);
  setText(0, name);
  setToolTip(0, TACTILE_QSTRING(u"(string)"));

  QObject::connect(m_widget,
                   &QLineEdit::textChanged,
                   [this](const QString& text) {
                     setText(1, text);
                   });

  Q_ASSERT(treeWidget());
  treeWidget()->setItemWidget(this, 1, m_widget);

  set_value(property);
}

void property_string_item::enable_focus_view()
{
  if (is_value_editable()) {
    m_widget->set_visible(true);
    m_widget->setFocus();
  }
}

void property_string_item::enable_idle_view()
{
  m_widget->set_visible(false);
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
  return m_widget;
}

}  // namespace tactile::gui

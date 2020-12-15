#include "property_tree_item.hpp"

#include <QColorDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <utility>  // move

#include "color_preview_button.hpp"
#include "color_validator.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {
namespace {

[[nodiscard]] auto inline_widget_for_type(const core::property::type type)
    -> QWidget*
{
  switch (type) {
    case core::property::string:
    case core::property::file:  // TODO line edit with "browse" button?
      [[fallthrough]];
    case core::property::object:  // TODO RO line edit with browsing options
      return nullptr;

    case core::property::color:
      return new color_preview_button{Qt::black};

    case core::property::integer: {
      auto* edit = new QLineEdit{};
      edit->setFrame(false);
      edit->setValidator(new QIntValidator{});
      return edit;
    }
    case core::property::floating: {
      auto* edit = new QLineEdit{};
      edit->setFrame(false);
      edit->setValidator(new QDoubleValidator{});
      return edit;
    }
    case core::property::boolean: {
      auto* box = new QComboBox{};
      box->addItem(TACTILE_QSTRING(u"false"));
      box->addItem(TACTILE_QSTRING(u"true"));
      box->setCurrentIndex(0);
      box->setFrame(false);
      return box;
    }
    default:
      throw tactile_error{"Did not recognize property type for item widget!"};
  }
}

[[nodiscard]] auto value_for_type(const core::property::type type) -> QString
{
  switch (type) {
    case core::property::string:
    case core::property::file:
    case core::property::color:
      [[fallthrough]];
    case core::property::object:
      return TACTILE_QSTRING(u"");

    case core::property::integer:
      return TACTILE_QSTRING(u"0");

    case core::property::floating:
      return TACTILE_QSTRING(u"0.0");

    case core::property::boolean:
      return TACTILE_QSTRING(u"false");

    default:
      throw tactile_error{"Did not recognize property type for value string!"};
  }
}

[[nodiscard]] auto tooltip_for_type(const core::property::type type) -> QString
{
  switch (type) {
    case core::property::string:
      return TACTILE_QSTRING(u"(string)");

    case core::property::file:
      return TACTILE_QSTRING(u"(file)");

    case core::property::object:
      return TACTILE_QSTRING(u"(object)");

    case core::property::color:
      return TACTILE_QSTRING(u"(color)");

    case core::property::integer:
      return TACTILE_QSTRING(u"(int)");

    case core::property::floating:
      return TACTILE_QSTRING(u"(float)");

    case core::property::boolean:
      return TACTILE_QSTRING(u"(bool)");

    default:
      throw tactile_error{"Did not recognize property type for tooltip text!"};
  }
}

}  // namespace

property_tree_item::property_tree_item(QTreeWidgetItem* parent)
    : QTreeWidgetItem{parent}
{}

property_tree_item::property_tree_item(const QString& name,
                                       const QString& value,
                                       QTreeWidgetItem* parent)
    : property_tree_item{parent}
{
  setText(0, name);
  setText(1, value);
}

property_tree_item::property_tree_item(const QString& name,
                                       const core::property::type type,
                                       QTreeWidgetItem* parent)
    : QTreeWidgetItem{parent}
    , m_isInlineProperty{type != core::property::color}
{
  setText(0, name);
  setToolTip(0, tooltip_for_type(type));

  if (m_isInlineProperty) {
    setText(1, value_for_type(type));
  }

  if (auto* widget = inline_widget_for_type(type)) {
    Q_ASSERT(treeWidget());
    widget->setAutoFillBackground(true);
    treeWidget()->setItemWidget(this, 1, widget);
  }

  if (type == core::property::color) {
    add_color_items();
  }
}

void property_tree_item::add_color_items()
{
  const auto addColor = [this](const QString& name, const int value) {
    auto* item = new property_tree_item{name, TACTILE_QSTRING(u"N/A"), this};

    auto* edit = new QLineEdit{};
    edit->setFrame(false);
    edit->setValidator(new color_validator{edit});
    edit->setText(QString::number(value));

    item->treeWidget()->setItemWidget(item, 1, edit);
    Q_ASSERT(edit->parent());  // ensure that there's no memory leak

    return edit;
  };

  if (auto* button = qobject_cast<color_preview_button*>(
          treeWidget()->itemWidget(this, 1))) {
    auto* red = addColor(TACTILE_QSTRING(u"Red"), 0);
    auto* green = addColor(TACTILE_QSTRING(u"Green"), 0);
    auto* blue = addColor(TACTILE_QSTRING(u"Blue"), 0);
    auto* alpha = addColor(TACTILE_QSTRING(u"Alpha"), 255);

    QObject::connect(button,
                     &color_preview_button::color_changed,
                     [=](const QColor& color) {
                       red->setText(QString::number(color.red()));
                       green->setText(QString::number(color.green()));
                       blue->setText(QString::number(color.blue()));
                       alpha->setText(QString::number(color.alpha()));
                     });

    QObject::connect(red,
                     &QLineEdit::textChanged,
                     [button](const QString& text) {
                       bool ok;
                       if (const auto result = text.toInt(&ok); ok) {
                         button->set_red(result);
                       }
                     });

    QObject::connect(green,
                     &QLineEdit::textChanged,
                     [button](const QString& text) {
                       bool ok;
                       if (const auto result = text.toInt(&ok); ok) {
                         button->set_green(result);
                       }
                     });

    QObject::connect(blue,
                     &QLineEdit::textChanged,
                     [button](const QString& text) {
                       bool ok;
                       if (const auto result = text.toInt(&ok); ok) {
                         button->set_blue(result);
                       }
                     });

    QObject::connect(alpha,
                     &QLineEdit::textChanged,
                     [button](const QString& text) {
                       bool ok;
                       if (const auto result = text.toInt(&ok); ok) {
                         button->set_alpha(result);
                       }
                     });
  }
}

auto property_tree_item::emplace_child(const QString& name,
                                       const QString& value)
    -> property_tree_item*
{
  auto* item = new property_tree_item{name, value};
  addChild(item);
  return item;
}

void property_tree_item::set_name_editable(const bool editable) noexcept
{
  m_nameEditable = editable;
}

auto property_tree_item::is_inline_property() const noexcept -> bool
{
  return m_isInlineProperty;
}

auto property_tree_item::is_name_editable() const noexcept -> bool
{
  return m_nameEditable;
}

}  // namespace tactile::gui

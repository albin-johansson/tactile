#pragma once

#include <QDoubleValidator>
#include <QIntValidator>
#include <QLineEdit>
#include <concepts>  // same_as

#include "property_tree_item.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

class number_value_widget final : public QLineEdit
{
 public:
  explicit number_value_widget(QWidget* parent = nullptr) : QLineEdit{parent}
  {
    setFrame(false);
    setAutoFillBackground(true);
  }

  void set_visible(const bool visible)
  {
    QLineEdit::setVisible(visible);
  }

  [[deprecated("See set_visible")]] void setVisible(bool visible) override
  {
    // This function does nothing by design as a workaround
  }
};

// clang-format off

template <typename T> requires(std::same_as<T, int> || std::same_as<T, double>)
class property_number_item final : public property_tree_item
{
  // clang-format on
 public:
  property_number_item(const QString& name,
                       const core::property& property,
                       QTreeWidgetItem* parent)
      : property_tree_item{name, parent}
      , m_edit{new number_value_widget{}}
  {
    if constexpr (std::same_as<T, int>) {
      setToolTip(0, TACTILE_QSTRING(u"(int)"));
    } else {
      setToolTip(0, TACTILE_QSTRING(u"(float)"));
    }

    if constexpr (std::same_as<T, int>) {
      m_edit->setValidator(new QIntValidator{});
    } else {
      m_edit->setValidator(new QDoubleValidator{});
    }

    QObject::connect(m_edit,
                     &QLineEdit::textChanged,
                     [this](const QString& text) {
                       setText(1, text);
                     });

    Q_ASSERT(treeWidget());
    treeWidget()->setItemWidget(this, 1, m_edit);

    set_value(property);
  }

  void enable_focus_view() override
  {
    if (is_value_editable()) {
      m_edit->set_visible(true);
      m_edit->setFocus();
    }
  }

  void enable_idle_view() override
  {
    m_edit->set_visible(false);
  }

  void set_value(const core::property& property) override
  {
    if (property.has_value()) {
      Q_ASSERT(property.is<T>());
      m_edit->setText(QString::number(property.as<T>()));
    } else {
      m_edit->setText(TACTILE_QSTRING(u"N/A"));
    }
  }

  [[nodiscard]] auto property_type() const noexcept
      -> core::property::type override
  {
    if constexpr (std::same_as<T, int>) {
      return core::property::integer;
    } else {
      return core::property::floating;
    }
  }

  [[nodiscard]] auto get_value_widget() -> number_value_widget* override
  {
    return nullptr;
  }

 private:
  number_value_widget* m_edit{};
};

}  // namespace tactile::gui

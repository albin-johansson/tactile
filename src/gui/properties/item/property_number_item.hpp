#pragma once

#include <QDoubleValidator>
#include <QIntValidator>
#include <QLineEdit>
#include <concepts>  // same_as

#include "property_tree_item.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

// clang-format off

template <typename T> requires(std::same_as<T, int> || std::same_as<T, double>)
class property_number_item final : public property_tree_item
{
  // clang-format on
 public:
  property_number_item(const QString& name,
                       const core::property& property,
                       QTreeWidgetItem* parent)
      : property_tree_item{parent}
  {
    Q_ASSERT(parent);
    setText(0, name);

    if constexpr (std::same_as<T, int>) {
      setToolTip(0, TACTILE_QSTRING(u"(int)"));
    } else {
      setToolTip(0, TACTILE_QSTRING(u"(float)"));
    }

    auto* edit = new QLineEdit{};
    edit->setFrame(false);
    if constexpr (std::same_as<T, int>) {
      edit->setValidator(new QIntValidator{});
    } else {
      edit->setValidator(new QDoubleValidator{});
    }
    edit->setAutoFillBackground(true);

    Q_ASSERT(treeWidget());
    treeWidget()->setItemWidget(this, 1, edit);

    set_value(property);
  }

  void set_value(const core::property& property) override
  {
    if (auto* edit = get_value_widget()) {
      if (property.has_value()) {
        Q_ASSERT(property.is<T>());
        edit->setText(QString::number(property.as<T>()));
      } else {
        edit->setText(TACTILE_QSTRING(u"N/A"));
      }
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

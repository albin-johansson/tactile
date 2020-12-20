#pragma once

#include <QLineEdit>

#include "property_tree_item.hpp"
#include "property_value_widget.hpp"

namespace tactile::gui {

class string_value_widget final : public QLineEdit
{
 public:
  explicit string_value_widget(QWidget* parent = nullptr);

  void set_visible(const bool visible)
  {
    QLineEdit::setVisible(visible);
  }

  [[deprecated("See set_visible")]] void setVisible(bool visible) override
  {
    // This function does nothing by design as a workaround
  }
};

class property_string_item final : public property_tree_item
{
 public:
  property_string_item(const QString& name,
                       const core::property& property,
                       QTreeWidgetItem* parent);

  void enable_focus_view() override;

  void enable_idle_view() override;

  void set_value(const core::property& property) override;

  [[nodiscard]] auto property_type() const noexcept
      -> core::property::type override;

 protected:
  [[nodiscard]] auto get_value_widget() -> QLineEdit* override;

 private:
  string_value_widget* m_widget{};
};

}  // namespace tactile::gui

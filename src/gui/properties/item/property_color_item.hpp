#pragma once

#include <QLineEdit>
#include <concepts>  // invocable

#include "property_tree_item.hpp"

namespace tactile::gui {

class color_preview_button;

class property_color_item final : public property_tree_item
{
 public:
  property_color_item(const QString& name,
                      const core::property& property,
                      QTreeWidgetItem* parent);

  void set_value(const core::property& property) override;

  [[nodiscard]] auto property_type() const noexcept
      -> core::property::type override;

  [[nodiscard]] auto get_value_widget() -> QWidget* override;

 private:
  void setup_color_preview(color_preview_button* preview);

  template <std::invocable<int> T>
  void connect_color_line_edit(QLineEdit* edit, T&& setColor)
  {
    QObject::connect(edit, &QLineEdit::textChanged, [=](const QString& text) {
      bool ok;
      if (const auto result = text.toInt(&ok); ok) {
        setColor(result);
      }
    });
  }
};

}  // namespace tactile::gui

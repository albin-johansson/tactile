#pragma once

#include <QWidget>

#include "map_document.hpp"
#include "property.hpp"

namespace Ui {
class properties_widget;
}

namespace tactile::gui {

class properties_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit properties_widget(QWidget* parent = nullptr);

 signals:
  void ui_add_property(const QString& name, const core::property& property);
  void ui_remove_property(const QString& name);
  void ui_rename_property(const QString& oldName, const QString& newName);

 public slots:
  void selected_map(const core::map_document& document);

  void select_layer(const core::layer& layer);

  void added_property(const QString& name, const core::property& property);

  void updated_property(const QString& name, const core::property& property);

  void removed_property(const QString& name);

 private:
  Ui::properties_widget* m_ui{};
};

}  // namespace tactile::gui

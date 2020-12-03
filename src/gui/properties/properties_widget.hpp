#pragma once

#include <QTreeWidgetItem>
#include <QWidget>

#include "map_document.hpp"
#include "property.hpp"
#include "vector_map.hpp"

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
  void ui_update_property(const QString& name, const core::property& property);
  void ui_rename_property(const QString& oldName, const QString& newName);

 public slots:
  void selected_map(const core::map_document& document);

  void select_layer(const core::layer& layer);

  void added_property(const QString& name, const core::property& property);

  void updated_property(const QString& name, const core::property& property);

  void removed_property(const QString& name);

 private:
  Ui::properties_widget* m_ui{};
  vector_map<QString, core::property> m_props;

 private slots:
  void upon_item_changed(QTreeWidgetItem* item, int column);

  void upon_item_double_clicked(QTreeWidgetItem* item, int column);
};

}  // namespace tactile::gui

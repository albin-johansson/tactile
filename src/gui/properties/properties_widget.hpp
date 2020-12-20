#pragma once

#include <QTreeWidgetItem>
#include <QWidget>

#include "map_document.hpp"
#include "maybe.hpp"
#include "property.hpp"
#include "tree_widget_item.hpp"

namespace Ui {
class properties_widget;
}

namespace tactile::gui {

class properties_context_menu;

class properties_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit properties_widget(QWidget* parent = nullptr);

 signals:
  void request_add_property(const QString& name, core::property::type type);
  void request_remove_property(const QString& name);

  void has_renamed_property(const QString& oldName, const QString& newName);
  void has_set_property(const QString& name, const core::property& property);

 public slots:
  void selected_map(const core::map_document& document);
  void select_layer(const core::tile_layer& layer);
  void added_property(const QString& name, const core::property& property);
  void removed_property(const QString& name);
  void moved_property_up(const QString& name);
  void moved_property_down(const QString& name);
  void duplicated_property(const QString& name);

 private:
  Ui::properties_widget* m_ui{};
  QTreeWidgetItem* m_predefinedRoot{};
  tree_widget_item* m_customRoot{};
  properties_context_menu* m_contextMenu{};
  maybe<QString> m_cachedName;

  void update_actions();

  void enable_idle_views();

  void trigger_context_menu(const QPoint& pos);

 private slots:
  void when_new_property_button_clicked();

  void when_remove_property_button_clicked();

  void when_item_selection_changed();

  void when_current_item_changed(QTreeWidgetItem* current,
                                 QTreeWidgetItem* previous);

  void when_item_double_clicked(QTreeWidgetItem* item, int column);

  void when_item_modified(QTreeWidgetItem* item, int column);
};

}  // namespace tactile::gui

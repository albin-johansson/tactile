#pragma once

#include "dock_widget.hpp"
#include "map_document.hpp"
#include "property.hpp"

namespace tactile::gui {

class properties_widget;

class properties_dock final : public dock_widget
{
  Q_OBJECT

 public:
  explicit properties_dock(QWidget* parent = nullptr);

 signals:
  void ui_add_property(const QString& name, core::property::type type);
  void ui_remove_property(const QString& name);
  void ui_update_property(const QString& name, const core::property& property);
  void ui_rename_property(const QString& oldName, const QString& newName);

 public slots:
  void selected_map(const core::map_document& document);

  void added_property(const QString& name, const core::property& property);

  void removed_property(const QString& name);

  void moved_property_up(const QString& name);

  void moved_property_down(const QString& name);

  void duplicated_property(const QString& name);

 private:
  properties_widget* m_widget{};
};

}  // namespace tactile::gui

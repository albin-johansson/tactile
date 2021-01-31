#pragma once

#include "dock_widget.hpp"
#include "map_document.hpp"
#include "property.hpp"
#include "property_model.hpp"
#include "smart_pointers.hpp"
#include "not_null.hpp"

namespace tactile::gui {

class properties_widget;

class properties_dock final : public dock_widget
{
  Q_OBJECT

 public:
  explicit properties_dock(QWidget* parent = nullptr);

 public slots:
  void switched_map(not_null<core::map_document*> document);

  void added_property(const QString& name);

  void about_to_remove_property(const QString& name);

  void updated_property(const QString& name);

  void changed_property_type(const QString& name);

  void renamed_property(const QString& oldName, const QString& newName);

 private:
  properties_widget* m_widget{};
};

}  // namespace tactile::gui

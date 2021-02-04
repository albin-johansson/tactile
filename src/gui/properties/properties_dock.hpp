#pragma once

#include "dock_widget.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::gui, properties_widget)

namespace tactile::gui {

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

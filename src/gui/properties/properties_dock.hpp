#pragma once

#include "dock_widget.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::core, property_manager)
TACTILE_FORWARD_DECLARE(tactile, PropertiesWidget)

namespace tactile {

class PropertiesDock final : public DockWidget
{
  Q_OBJECT

 public:
  explicit PropertiesDock(QWidget* parent = nullptr);

  void ShowMapProperties(not_null<core::property_manager*> manager);

  void ShowLayerProperties(not_null<core::property_manager*> manager);

 public slots:
  void OnSwitchedMap(not_null<core::property_manager*> manager);

  void OnAddedProperty(const QString& name);

  void OnAboutToRemoveProperty(const QString& name);

  void OnUpdatedProperty(const QString& name);

  void OnChangedPropertyType(const QString& name);

  void OnRenamedProperty(const QString& oldName, const QString& newName);

 private:
  PropertiesWidget* mWidget{};
};

}  // namespace tactile

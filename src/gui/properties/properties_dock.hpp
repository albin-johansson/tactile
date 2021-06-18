#pragma once

#include "dock_widget.hpp"
#include "fwd.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"
#include "tactile_declare_ui.hpp"

namespace tactile {

class PropertiesDock final : public DockWidget
{
  Q_OBJECT

 public:
  explicit PropertiesDock(QWidget* parent = nullptr);

 public slots:
  void OnSwitchedMap(NotNull<core::IPropertyManager*> manager);

  void OnAddedProperty(const QString& name);

  void OnAboutToRemoveProperty(const QString& name);

  void OnUpdatedProperty(const QString& name);

  void OnChangedPropertyType(const QString& name);

  void OnRenamedProperty(const QString& oldName, const QString& newName);

  void OnUpdatedPropertyContext(NotNull<core::IPropertyManager*> context,
                                const QString& name);

 private:
  PropertiesWidget* mWidget{};
};

}  // namespace tactile

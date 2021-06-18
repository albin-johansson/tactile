#include "properties_dock.hpp"

#include "properties_widget.hpp"

namespace tactile {

PropertiesDock::PropertiesDock(QWidget* parent)
    : DockWidget{parent}
    , mWidget{new PropertiesWidget{this}}
{
  setObjectName(QStringLiteral(u"PropertiesDock"));
  setWindowTitle(tr("Properties"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(mWidget);
}

void PropertiesDock::OnSwitchedMap(NotNull<core::IPropertyManager*> manager)
{
  mWidget->ShowMap(manager);
}

void PropertiesDock::ShowMapProperties(NotNull<core::IPropertyManager*> manager)
{
  mWidget->ShowMap(manager);
}

void PropertiesDock::ShowLayerProperties(
    NotNull<core::IPropertyManager*> manager)
{
  mWidget->ShowLayer(manager);
}

void PropertiesDock::ShowTilesetProperties(
    NotNull<core::IPropertyManager*> manager)
{
  mWidget->ShowTileset(manager);
}

void PropertiesDock::OnAddedProperty(const QString& name)
{
  mWidget->OnAddedProperty(name);
}

void PropertiesDock::OnAboutToRemoveProperty(const QString& name)
{
  mWidget->OnAboutToRemoveProperty(name);
}

void PropertiesDock::OnUpdatedProperty(const QString& name)
{
  mWidget->OnUpdatedProperty(name);
}

void PropertiesDock::OnChangedPropertyType(const QString& name)
{
  mWidget->OnChangedPropertyType(name);
}

void PropertiesDock::OnRenamedProperty(const QString& oldName,
                                       const QString& newName)
{
  mWidget->OnRenamedProperty(oldName, newName);
}

void PropertiesDock::OnUpdatedPropertyContext(core::IPropertyManager* context,
                                              const QStringView name)
{
  mWidget->OnUpdatedPropertyContext(context, name);
}

}  // namespace tactile

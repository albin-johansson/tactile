#include "layer_delegate.hpp"

#include <algorithm>  // clamp
#include <utility>    // move

namespace tactile::core {

LayerDelegate::LayerDelegate(const LayerType type) noexcept : mType{type}
{}

void LayerDelegate::SetVisible(const bool visible) noexcept
{
  mVisible = visible;
}

void LayerDelegate::SetOpacity(const double opacity)
{
  mOpacity = std::clamp(opacity, 0.0, 1.0);
}

void LayerDelegate::SetName(QString name)
{
  mName = std::move(name);
}

auto LayerDelegate::Type() const noexcept -> LayerType
{
  return mType;
}

auto LayerDelegate::Visible() const noexcept -> bool
{
  return mVisible;
}

auto LayerDelegate::Opacity() const noexcept -> double
{
  return mOpacity;
}

auto LayerDelegate::Name() const -> const QString&
{
  return mName;
}

void LayerDelegate::AddProperty(const QString& name, const PropertyType type)
{
  mProperties.AddProperty(name, type);
}

void LayerDelegate::AddProperty(const QString& name, const Property& property)
{
  mProperties.AddProperty(name, property);
}

void LayerDelegate::RemoveProperty(const QString& name)
{
  mProperties.RemoveProperty(name);
}

void LayerDelegate::RenameProperty(const QString& oldName,
                                   const QString& newName)
{
  mProperties.RenameProperty(oldName, newName);
}

void LayerDelegate::SetProperty(const QString& name, const Property& property)
{
  mProperties.SetProperty(name, property);
}

void LayerDelegate::ChangePropertyType(const QString& name,
                                       const PropertyType type)
{
  mProperties.ChangePropertyType(name, type);
}

auto LayerDelegate::GetProperty(const QString& name) const -> const Property&
{
  return mProperties.GetProperty(name);
}

auto LayerDelegate::GetProperty(const QString& name) -> Property&
{
  return mProperties.GetProperty(name);
}

auto LayerDelegate::HasProperty(const QString& name) const -> bool
{
  return mProperties.HasProperty(name);
}

auto LayerDelegate::PropertyCount() const noexcept -> int
{
  return mProperties.PropertyCount();
}

auto LayerDelegate::Properties() const -> const IPropertyManager::property_map&
{
  return mProperties.GetProperties();
}

}  // namespace tactile::core

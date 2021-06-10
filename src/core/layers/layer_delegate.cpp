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

void LayerDelegate::AddProperty(const QString& name, property_type type)
{
  mProperties.add_property(name, type);
}

void LayerDelegate::AddProperty(const QString& name, const property& property)
{
  mProperties.add_property(name, property);
}

void LayerDelegate::RemoveProperty(const QString& name)
{
  mProperties.remove_property(name);
}

void LayerDelegate::RenameProperty(const QString& oldName,
                                   const QString& newName)
{
  mProperties.rename_property(oldName, newName);
}

void LayerDelegate::SetProperty(const QString& name, const property& property)
{
  mProperties.set_property(name, property);
}

void LayerDelegate::ChangePropertyType(const QString& name, property_type type)
{
  mProperties.change_property_type(name, type);
}

auto LayerDelegate::GetProperty(const QString& name) const -> const property&
{
  return mProperties.get_property(name);
}

auto LayerDelegate::GetProperty(const QString& name) -> property&
{
  return mProperties.get_property(name);
}

auto LayerDelegate::HasProperty(const QString& name) const -> bool
{
  return mProperties.has_property(name);
}

auto LayerDelegate::PropertyCount() const noexcept -> int
{
  return mProperties.property_count();
}

auto LayerDelegate::Properties() const -> const property_manager::property_map&
{
  return mProperties.properties();
}

}  // namespace tactile::core

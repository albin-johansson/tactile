#include "abstract_layer.hpp"

#include <utility>  // move

namespace Tactile {

ALayer::ALayer(const LayerType type) : mDelegate{type}
{}

void ALayer::SetVisible(const bool visible)
{
  mDelegate.SetVisible(visible);
}

void ALayer::SetOpacity(const float opacity)
{
  mDelegate.SetOpacity(opacity);
}

void ALayer::SetName(std::string name)
{
  mDelegate.SetName(std::move(name));
}

auto ALayer::IsVisible() const -> bool
{
  return mDelegate.IsVisible();
}

auto ALayer::GetOpacity() const noexcept -> float
{
  return mDelegate.GetOpacity();
}

auto ALayer::GetType() const -> LayerType
{
  return mDelegate.GetType();
}

void ALayer::AddProperty(std::string name, const PropertyType type)
{
  mDelegate.AddProperty(std::move(name), type);
}

void ALayer::AddProperty(std::string name, const Property& property)
{
  mDelegate.AddProperty(std::move(name), property);
}

void ALayer::RemoveProperty(const std::string_view name)
{
  mDelegate.RemoveProperty(name);
}

void ALayer::RenameProperty(const std::string_view oldName, std::string newName)
{
  mDelegate.RenameProperty(oldName, std::move(newName));
}

void ALayer::SetProperty(const std::string_view name, const Property& property)
{
  mDelegate.SetProperty(name, property);
}

void ALayer::ChangePropertyType(std::string name, const PropertyType type)
{
  mDelegate.ChangePropertyType(std::move(name), type);
}

auto ALayer::HasProperty(const std::string_view name) const -> bool
{
  return mDelegate.HasProperty(name);
}

auto ALayer::GetProperty(const std::string_view name) const -> const Property&
{
  return mDelegate.GetProperty(name);
}

auto ALayer::GetProperties() const -> const PropertyMap&
{
  return mDelegate.GetProperties();
}

auto ALayer::GetPropertyCount() const -> usize
{
  return mDelegate.GetPropertyCount();
}

auto ALayer::GetName() const -> const std::string&
{
  return mDelegate.GetName();
}

}  // namespace Tactile

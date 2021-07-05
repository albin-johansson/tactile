#include "object.hpp"

namespace Tactile {

void Object::AddProperty(const std::string& name, const PropertyType type)
{
  mProperties.AddProperty(name, type);
}

void Object::AddProperty(const std::string& name, const Property& property)
{
  mProperties.AddProperty(name, property);
}

void Object::RemoveProperty(const std::string_view name)
{
  mProperties.RemoveProperty(name);
}

void Object::RenameProperty(const std::string_view oldName,
                            const std::string& newName)
{
  mProperties.RenameProperty(oldName, newName);
}

void Object::SetProperty(const std::string_view name, const Property& property)
{
  mProperties.SetProperty(name, property);
}

void Object::ChangePropertyType(const std::string_view name, const PropertyType type)
{
  mProperties.ChangePropertyType(name, type);
}

auto Object::HasProperty(const std::string_view name) const -> bool
{
  return mProperties.HasProperty(name);
}

auto Object::GetProperty(const std::string_view name) const -> const Property&
{
  return mProperties.GetProperty(name);
}

auto Object::GetProperties() const -> const PropertyMap&
{
  return mProperties.GetProperties();
}

auto Object::GetPropertyCount() const -> usize
{
  return mProperties.GetPropertyCount();
}

auto Object::GetName() const -> std::string_view
{
  return mProperties.GetName();
}

}  // namespace Tactile

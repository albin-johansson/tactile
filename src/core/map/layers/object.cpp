#include "object.hpp"

#include <utility>  // move

namespace Tactile {

void Object::AddProperty(std::string name, const PropertyType type)
{
  mProperties.AddProperty(std::move(name), type);
}

void Object::AddProperty(std::string name, const Property& property)
{
  mProperties.AddProperty(std::move(name), property);
}

void Object::RemoveProperty(const std::string_view name)
{
  mProperties.RemoveProperty(name);
}

void Object::RenameProperty(const std::string_view oldName, std::string newName)
{
  mProperties.RenameProperty(oldName, std::move(newName));
}

void Object::SetProperty(const std::string_view name, const Property& property)
{
  mProperties.SetProperty(name, property);
}

void Object::ChangePropertyType(std::string name, const PropertyType type)
{
  mProperties.ChangePropertyType(std::move(name), type);
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

auto Object::GetName() const -> const std::string&
{
  return mProperties.GetName();
}

}  // namespace Tactile

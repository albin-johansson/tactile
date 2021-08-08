#include "property_delegate.hpp"

#include <cassert>        // assert
#include <centurion.hpp>  // log
#include <utility>        // move

namespace Tactile {

PropertyDelegate::PropertyDelegate(std::string name) : mName{std::move(name)}
{}

void PropertyDelegate::SetName(std::string name)
{
  mName = std::move(name);
}

void PropertyDelegate::AddProperty(std::string name, const PropertyType type)
{
  assert(!mProperties.contains(name));

  Property property;
  property.ResetToDefault(type);

  mProperties.emplace(std::move(name), std::move(property));
}

void PropertyDelegate::AddProperty(std::string name, const Property& property)
{
  assert(!mProperties.contains(name));
  assert(property.GetType());

  mProperties.emplace(std::move(name), property);
}

void PropertyDelegate::RemoveProperty(const std::string_view name)
{
  assert(mProperties.contains(name));
  mProperties.erase(name);

  CENTURION_LOG_DEBUG("PropertyDelegate::RemoveProperty: Removed %s", name.data());
}

void PropertyDelegate::RenameProperty(const std::string_view oldName,
                                      std::string newName)
{
  assert(mProperties.contains(oldName));
  assert(!mProperties.contains(newName));

  auto property = mProperties.at(oldName);
  mProperties.erase(oldName);
  mProperties.emplace(std::move(newName), std::move(property));
}

void PropertyDelegate::SetProperty(const std::string_view name,
                                   const Property& property)
{
  assert(mProperties.contains(name));
  assert(mProperties.at(name).GetType() == property.GetType());
  mProperties.at(name) = property;
}

void PropertyDelegate::ChangePropertyType(std::string name, const PropertyType type)
{
  assert(mProperties.contains(name));
  assert(GetProperty(name).GetType() != type);

  mProperties.erase(name);

  Property property;
  property.ResetToDefault(type);

  mProperties.emplace(std::move(name), std::move(property));
}

auto PropertyDelegate::HasProperty(const std::string_view name) const -> bool
{
  return mProperties.contains(name);
}

auto PropertyDelegate::GetProperty(const std::string_view name) const
    -> const Property&
{
  return mProperties.at(name);
}

auto PropertyDelegate::GetProperties() const -> const PropertyMap&
{
  return mProperties;
}

auto PropertyDelegate::GetPropertyCount() const -> usize
{
  return mProperties.size();
}

auto PropertyDelegate::GetName() const -> const std::string&
{
  return mName;
}

}  // namespace Tactile

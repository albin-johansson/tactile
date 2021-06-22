#include "property_delegate.hpp"

#include <cassert>        // assert
#include <centurion.hpp>  // log
#include <utility>        // move

namespace tactile {

PropertyDelegate::PropertyDelegate(std::string name) : mName{std::move(name)}
{}

void PropertyDelegate::SetName(std::string name)
{
  mName = std::move(name);
}

void PropertyDelegate::AddProperty(const std::string& name,
                                   const PropertyType type)
{
  assert(!mProperties.contains(name));

  Property property;
  property.ResetToDefault(type);

  mProperties.emplace(name, std::move(property));

  CENTURION_LOG_DEBUG("%s: Added %s with type %i",
                      __FUNCTION__,
                      name.c_str(),
                      cen::to_underlying(type));
}

void PropertyDelegate::AddProperty(const std::string& name,
                                   const Property& property)
{
  assert(!mProperties.contains(name));
  assert(property.GetType());

  mProperties.emplace(name, property);

  CENTURION_LOG_DEBUG("%s: Added %s with type %i",
                      __FUNCTION__,
                      name.c_str(),
                      cen::to_underlying(property.GetType().value()));
}

void PropertyDelegate::RemoveProperty(const std::string_view name)
{
  assert(mProperties.contains(name));
  mProperties.erase(name);

  CENTURION_LOG_DEBUG("PropertyDelegate::RemoveProperty: Removed %s",
                      name.data());
}

void PropertyDelegate::RenameProperty(const std::string_view oldName,
                                      const std::string& newName)
{
  assert(mProperties.contains(oldName));
  assert(!mProperties.contains(newName));

  auto property = mProperties.at(oldName);
  mProperties.erase(oldName);
  mProperties.emplace(newName, std::move(property));

  CENTURION_LOG_DEBUG("PropertyDelegate::RenameProperty: %s -> %s",
                      oldName.data(),
                      newName.c_str());
}

void PropertyDelegate::SetProperty(const std::string_view name,
                                   const Property& property)
{
  assert(mProperties.contains(name));
  assert(mProperties.at(name).GetType() == property.GetType());
  mProperties.at(name) = property;

  CENTURION_LOG_DEBUG("Updated value of property \"%s\"", name.data());
}

void PropertyDelegate::ChangePropertyType(const std::string_view name,
                                          const PropertyType type)
{
  assert(mProperties.contains(name));
  assert(GetProperty(name).GetType() != type);

  mProperties.erase(name);

  Property property;
  property.ResetToDefault(type);

  mProperties.emplace(std::string{name}, std::move(property));

  CENTURION_LOG_DEBUG("Changed type of property \"%s\" to %i",
                      name.data(),
                      cen::to_underlying(type));
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

auto PropertyDelegate::GetName() const -> std::string_view
{
  return mName;
}

}  // namespace tactile

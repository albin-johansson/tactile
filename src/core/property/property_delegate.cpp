#include "property_delegate.hpp"

#include <utility>  // move

#include "to_underlying.hpp"

namespace tactile::core {

PropertyDelegate::PropertyDelegate(const QString& name) : mName{name}
{}

void PropertyDelegate::AddProperty(const QString& name, const PropertyType type)
{
  Q_ASSERT(!mProperties.contains(name));

  Property property;
  property.SetDefault(type);

  mProperties.emplace(name, std::move(property));

#ifdef QT_DEBUG
  qDebug() << "PropertyDelegate::AddProperty: Added" << name
           << "with type:" << ToUnderlying(type);
#endif  // QT_DEBUG
}

void PropertyDelegate::AddProperty(const QString& name,
                                   const Property& property)
{
  Q_ASSERT(!mProperties.contains(name));
  Q_ASSERT(property.Type());

  mProperties.emplace(name, property);

#ifdef QT_DEBUG
  qDebug() << "PropertyDelegate::AddProperty: Added" << name << "with type"
           << ToUnderlying(property.Type().value());
#endif  // QT_DEBUG
}

void PropertyDelegate::RemoveProperty(const QString& name)
{
  Q_ASSERT(mProperties.contains(name));
  mProperties.erase(name);

#ifdef QT_DEBUG
  qDebug() << "PropertyDelegate::RemoveProperty: Removed" << name;
#endif  // QT_DEBUG
}

void PropertyDelegate::RenameProperty(const QString& oldName,
                                      const QString& newName)
{
  Q_ASSERT(mProperties.contains(oldName));
  Q_ASSERT(!mProperties.contains(newName));

  auto property = mProperties.at(oldName);
  mProperties.erase(oldName);
  mProperties.emplace(newName, std::move(property));

#ifdef QT_DEBUG
  qDebug() << "PropertyDelegate::RenameProperty:" << oldName << "->" << newName;
#endif  // QT_DEBUG
}

void PropertyDelegate::SetProperty(const QString& name,
                                   const Property& property)
{
  Q_ASSERT(mProperties.contains(name));
  Q_ASSERT(mProperties.at(name).Type() == property.Type());
  mProperties.at(name) = property;

#ifdef QT_DEBUG
  qDebug() << "PropertyDelegate::SetProperty: Updated" << name;
#endif  // QT_DEBUG
}

void PropertyDelegate::ChangePropertyType(const QString& name,
                                          const PropertyType type)
{
  Q_ASSERT(mProperties.contains(name));
  Q_ASSERT(GetProperty(name).Type() != type);

  mProperties.erase(name);

  Property property;
  property.SetDefault(type);

  mProperties.emplace(name, std::move(property));

#ifdef QT_DEBUG
  qDebug() << "PropertyDelegate::ChangePropertyType: Changed type of" << name
           << "to" << ToUnderlying(type);
#endif  // QT_DEBUG
}

auto PropertyDelegate::GetProperty(const QString& name) const -> const Property&
{
  return mProperties.at(name);
}

auto PropertyDelegate::GetProperty(const QString& name) -> Property&
{
  return mProperties.at(name);
}

auto PropertyDelegate::HasProperty(const QString& name) const -> bool
{
  return mProperties.contains(name);
}

auto PropertyDelegate::PropertyCount() const noexcept -> int
{
  return static_cast<int>(mProperties.size());
}

auto PropertyDelegate::GetProperties() const -> const property_map&
{
  return mProperties;
}

auto PropertyDelegate::GetName() const -> QString
{
  return mName;
}

}  // namespace tactile::core

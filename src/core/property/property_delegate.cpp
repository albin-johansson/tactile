#include "property_delegate.hpp"

#include <utility>  // move

namespace tactile::core {

void property_delegate::AddProperty(const QString& name,
                                    const PropertyType type)
{
  Q_ASSERT(!mProperties.contains(name));

  Property property;
  property.SetDefault(type);

  mProperties.emplace(name, std::move(property));

#ifdef QT_DEBUG
  qDebug() << "Added property:" << name
           << "with type:" << static_cast<int>(type);
#endif  // QT_DEBUG
}

void property_delegate::AddProperty(const QString& name,
                                    const Property& property)
{
  Q_ASSERT(!mProperties.contains(name));
  Q_ASSERT(property.Type());

  mProperties.emplace(name, property);

#ifdef QT_DEBUG
  qDebug() << "Added property:" << name
           << "with type:" << static_cast<int>(property.Type().value());
#endif  // QT_DEBUG
}

void property_delegate::RemoveProperty(const QString& name)
{
  Q_ASSERT(mProperties.contains(name));
  mProperties.erase(name);

#ifdef QT_DEBUG
  qDebug() << "Removed property:" << name;
#endif  // QT_DEBUG
}

void property_delegate::RenameProperty(const QString& oldName,
                                       const QString& newName)
{
  Q_ASSERT(mProperties.contains(oldName));
  Q_ASSERT(!mProperties.contains(newName));

  auto property = mProperties.at(oldName);
  mProperties.erase(oldName);
  mProperties.emplace(newName, std::move(property));

#ifdef QT_DEBUG
  qDebug() << "Renamed property:" << oldName << "->" << newName;
#endif  // QT_DEBUG
}

void property_delegate::SetProperty(const QString& name,
                                    const Property& property)
{
  Q_ASSERT(mProperties.contains(name));
  Q_ASSERT(mProperties.at(name).Type() == property.Type());
  mProperties.at(name) = property;

#ifdef QT_DEBUG
  qDebug() << "Set value of property:" << name;
#endif  // QT_DEBUG
}

void property_delegate::ChangePropertyType(const QString& name,
                                           const PropertyType type)
{
  Q_ASSERT(mProperties.contains(name));
  Q_ASSERT(GetProperty(name).Type() != type);

  mProperties.erase(name);

  Property property;
  property.SetDefault(type);

  mProperties.emplace(name, std::move(property));

#ifdef QT_DEBUG
  qDebug() << "Changed type of property:" << name << "to"
           << static_cast<int>(type);
#endif  // QT_DEBUG
}

auto property_delegate::GetProperty(const QString& name) const
    -> const Property&
{
  return mProperties.at(name);
}

auto property_delegate::GetProperty(const QString& name) -> Property&
{
  return mProperties.at(name);
}

auto property_delegate::HasProperty(const QString& name) const -> bool
{
  return mProperties.contains(name);
}

auto property_delegate::PropertyCount() const noexcept -> int
{
  return static_cast<int>(mProperties.size());
}

auto property_delegate::GetProperties() const -> const property_map&
{
  return mProperties;
}

}  // namespace tactile::core

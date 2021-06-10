#include "property_delegate.hpp"

#include <utility>  // move

namespace tactile::core {

void property_delegate::AddProperty(const QString& name, property_type type)
{
  Q_ASSERT(!m_properties.contains(name));

  property p;
  p.set_default(type);

  m_properties.emplace(name, std::move(p));

#ifdef QT_DEBUG
  qDebug() << "Added property:" << name
           << "with type:" << static_cast<int>(type);
#endif  // QT_DEBUG
}

void property_delegate::AddProperty(const QString& name,
                                     const property& property)
{
  Q_ASSERT(!m_properties.contains(name));
  Q_ASSERT(property.type());

  m_properties.emplace(name, property);

#ifdef QT_DEBUG
  qDebug() << "Added property:" << name
           << "with type:" << static_cast<int>(property.type().value());
#endif  // QT_DEBUG
}

void property_delegate::RemoveProperty(const QString& name)
{
  Q_ASSERT(m_properties.contains(name));
  m_properties.erase(name);

#ifdef QT_DEBUG
  qDebug() << "Removed property:" << name;
#endif  // QT_DEBUG
}

void property_delegate::RenameProperty(const QString& oldName,
                                        const QString& newName)
{
  Q_ASSERT(m_properties.contains(oldName));
  Q_ASSERT(!m_properties.contains(newName));

  auto property = m_properties.at(oldName);
  m_properties.erase(oldName);
  m_properties.emplace(newName, std::move(property));

#ifdef QT_DEBUG
  qDebug() << "Renamed property:" << oldName << "->" << newName;
#endif  // QT_DEBUG
}

void property_delegate::SetProperty(const QString& name,
                                     const property& property)
{
  Q_ASSERT(m_properties.contains(name));
  Q_ASSERT(m_properties.at(name).type() == property.type());
  m_properties.at(name) = property;

#ifdef QT_DEBUG
  qDebug() << "Set value of property:" << name;
#endif  // QT_DEBUG
}

void property_delegate::ChangePropertyType(const QString& name,
                                           property_type type)
{
  Q_ASSERT(m_properties.contains(name));
  Q_ASSERT(GetProperty(name).type() != type);

  m_properties.erase(name);

  property p;
  p.set_default(type);

  m_properties.emplace(name, std::move(p));

#ifdef QT_DEBUG
  qDebug() << "Changed type of property:" << name << "to"
           << static_cast<int>(type);
#endif  // QT_DEBUG
}

auto property_delegate::GetProperty(const QString& name) const
    -> const property&
{
  return m_properties.at(name);
}

auto property_delegate::GetProperty(const QString& name) -> property&
{
  return m_properties.at(name);
}

auto property_delegate::HasProperty(const QString& name) const -> bool
{
  return m_properties.contains(name);
}

auto property_delegate::PropertyCount() const noexcept -> int
{
  return static_cast<int>(m_properties.size());
}

auto property_delegate::GetProperties() const -> const property_map&
{
  return m_properties;
}

}  // namespace tactile::core

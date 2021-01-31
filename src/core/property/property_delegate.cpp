#include "property_delegate.hpp"

#include <utility>  // move

#ifdef QT_DEBUG
#include <QDebug>
#endif  // QT_DEBUG

namespace tactile::core {

void property_delegate::add_property(const QString& name,
                                     const property::type type)
{
  Q_ASSERT(!m_properties.contains(name));

  property p;
  p.set_default(type);

  m_properties.emplace(name, std::move(p));

#ifdef QT_DEBUG
  qDebug() << "Added property:" << name << "with type:" << type;
#endif  // QT_DEBUG
}

void property_delegate::add_property(const QString& name,
                                     const property& property)
{
  Q_ASSERT(!m_properties.contains(name));
  Q_ASSERT(property.get_type());

  m_properties.emplace(name, property);

#ifdef QT_DEBUG
  qDebug() << "Added property:" << name
           << "with type:" << property.get_type().value();
#endif  // QT_DEBUG
}

void property_delegate::remove_property(const QString& name)
{
  Q_ASSERT(m_properties.contains(name));
  m_properties.erase(name);

#ifdef QT_DEBUG
  qDebug() << "Removed property:" << name;
#endif  // QT_DEBUG
}

void property_delegate::rename_property(const QString& oldName,
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

void property_delegate::set_property(const QString& name,
                                     const property& property)
{
  Q_ASSERT(m_properties.contains(name));
  Q_ASSERT(m_properties.at(name).get_type() == property.get_type());
  m_properties.at(name) = property;

#ifdef QT_DEBUG
  qDebug() << "Set value of property:" << name;
#endif  // QT_DEBUG
}

void property_delegate::change_property_type(const QString& name,
                                             const core::property::type type)
{
  Q_ASSERT(m_properties.contains(name));
  Q_ASSERT(get_property(name).get_type() != type);

  m_properties.erase(name);

  property p;
  p.set_default(type);

  m_properties.emplace(name, std::move(p));

#ifdef QT_DEBUG
  qDebug() << "Changed type of property:" << name << "to" << type;
#endif  // QT_DEBUG
}

auto property_delegate::get_property(const QString& name) const
    -> const property&
{
  return m_properties.at(name);
}

auto property_delegate::get_property(const QString& name) -> property&
{
  return m_properties.at(name);
}

auto property_delegate::has_property(const QString& name) const -> bool
{
  return m_properties.contains(name);
}

auto property_delegate::property_count() const noexcept -> int
{
  return static_cast<int>(m_properties.size());
}

auto property_delegate::properties() const -> const property_map&
{
  return m_properties;
}

}  // namespace tactile::core

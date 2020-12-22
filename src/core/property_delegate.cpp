#include "property_delegate.hpp"

#include <utility>  // move


namespace tactile::core {

void property_delegate::add_property(const QString& name,
                                     const property::type type)
{
  Q_ASSERT(!m_properties.contains(name));

  property p;
  p.set_default(type);

  m_properties.emplace(name, std::move(p));

}

void property_delegate::remove_property(const QString& name)
{
  Q_ASSERT(m_properties.contains(name));
  m_properties.erase(name);

}

void property_delegate::rename_property(const QString& oldName,
                                        const QString& newName)
{
  Q_ASSERT(m_properties.contains(oldName));
  Q_ASSERT(!m_properties.contains(newName));

  auto property = m_properties.at(oldName);
  m_properties.emplace(newName, std::move(property));

}

void property_delegate::set_property(const QString& name,
                                     const property& property)
{
  Q_ASSERT(m_properties.contains(name));
  m_properties.at(name) = property;

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

auto property_delegate::property_count() const noexcept -> int
{
  return static_cast<int>(m_properties.size());
}

}  // namespace tactile::core

#include "object_layer.hpp"

#include <utility>  // move

namespace tactile::core {

void object_layer::add_property(const QString& name, const property::type type)
{
  m_propertyDelegate.add_property(name, type);
}

void object_layer::remove_property(const QString& name)
{
  m_propertyDelegate.remove_property(name);
}

void object_layer::rename_property(const QString& oldName,
                                   const QString& newName)
{
  m_propertyDelegate.rename_property(oldName, newName);
}

void object_layer::set_property(const QString& name, const property& property)
{
  m_propertyDelegate.set_property(name, property);
}

auto object_layer::get_property(const QString& name) const -> const property&
{
  return m_propertyDelegate.get_property(name);
}

auto object_layer::get_property(const QString& name) -> property&
{
  return m_propertyDelegate.get_property(name);
}

auto object_layer::property_count() const noexcept -> int
{
  return m_propertyDelegate.property_count();
}

void object_layer::set_visible(const bool visible) noexcept
{
  m_layerDelegate.set_visible(visible);
}

void object_layer::set_opacity(const double opacity)
{
  m_layerDelegate.set_opacity(opacity);
}

void object_layer::set_name(QString name)
{
  m_layerDelegate.set_name(std::move(name));
}

auto object_layer::visible() const noexcept -> bool
{
  return m_layerDelegate.visible();
}

auto object_layer::opacity() const noexcept -> double
{
  return m_layerDelegate.opacity();
}

auto object_layer::name() const -> const QString&
{
  return m_layerDelegate.name();
}

}  // namespace tactile::core

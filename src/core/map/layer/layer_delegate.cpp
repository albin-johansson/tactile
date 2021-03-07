#include "layer_delegate.hpp"

#include <algorithm>  // clamp
#include <utility>    // move

namespace tactile::core {

layer_delegate::layer_delegate(const layer_type type) noexcept : m_type{type}
{}

void layer_delegate::set_visible(const bool visible) noexcept
{
  m_visible = visible;
}

void layer_delegate::set_opacity(const double opacity)
{
  m_opacity = std::clamp(opacity, 0.0, 1.0);
}

void layer_delegate::set_name(QString name)
{
  m_name = std::move(name);
}

auto layer_delegate::type() const noexcept -> layer_type
{
  return m_type;
}

auto layer_delegate::visible() const noexcept -> bool
{
  return m_visible;
}

auto layer_delegate::opacity() const noexcept -> double
{
  return m_opacity;
}

auto layer_delegate::name() const -> const QString&
{
  return m_name;
}

void layer_delegate::add_property(const QString& name, const property_type type)
{
  m_properties.add_property(name, type);
}

void layer_delegate::add_property(const QString& name, const property& property)
{
  m_properties.add_property(name, property);
}

void layer_delegate::remove_property(const QString& name)
{
  m_properties.remove_property(name);
}

void layer_delegate::rename_property(const QString& oldName,
                                     const QString& newName)
{
  m_properties.rename_property(oldName, newName);
}

void layer_delegate::set_property(const QString& name, const property& property)
{
  m_properties.set_property(name, property);
}

void layer_delegate::change_property_type(const QString& name,
                                          const property_type type)
{
  m_properties.change_property_type(name, type);
}

auto layer_delegate::get_property(const QString& name) const -> const property&
{
  return m_properties.get_property(name);
}

auto layer_delegate::get_property(const QString& name) -> property&
{
  return m_properties.get_property(name);
}

auto layer_delegate::has_property(const QString& name) const -> bool
{
  return m_properties.has_property(name);
}

auto layer_delegate::property_count() const noexcept -> int
{
  return m_properties.property_count();
}

auto layer_delegate::properties() const -> const property_manager::property_map&
{
  return m_properties.properties();
}

}  // namespace tactile::core

#include "object.hpp"

#include <utility>  // move

namespace tactile::core {

object::object(const object_type type) : m_type{type}
{}

void object::set_x(const double x) noexcept
{
  m_x = x;
}

void object::set_y(const double y) noexcept
{
  m_y = y;
}

void object::set_width(const double width) noexcept
{
  m_width = width;
}

void object::set_height(const double height) noexcept
{
  m_height = height;
}

void object::set_visible(const bool visible) noexcept
{
  m_visible = visible;
}

void object::set_name(const QString& name)
{
  m_name = name;
}

void object::set_custom_type(maybe<QString> type)
{
  m_customType = std::move(type);
}

auto object::x() const noexcept -> double
{
  return m_x;
}

auto object::y() const noexcept -> double
{
  return m_y;
}

auto object::width() const noexcept -> double
{
  return m_width;
}

auto object::height() const noexcept -> double
{
  return m_height;
}

auto object::visible() const noexcept -> bool
{
  return m_visible;
}

auto object::name() const -> QString
{
  return m_name;
}

auto object::type() const noexcept -> object_type
{
  return m_type;
}

auto object::custom_type() const -> maybe<QString>
{
  return m_customType;
}

auto object::is_point() const noexcept -> bool
{
  return type() == object_type::point;
}

auto object::is_rectangle() const noexcept -> bool
{
  return type() == object_type::rectangle;
}

void object::add_property(const QString& name, const property::type type)
{
  m_properties.add_property(name, type);
}

void object::add_property(const QString& name, const property& property)
{
  m_properties.add_property(name, property);
}

void object::remove_property(const QString& name)
{
  m_properties.remove_property(name);
}

void object::rename_property(const QString& oldName, const QString& newName)
{
  m_properties.rename_property(oldName, newName);
}

void object::set_property(const QString& name, const property& property)
{
  m_properties.set_property(name, property);
}

void object::change_property_type(const QString& name,
                                  const core::property::type type)
{
  m_properties.change_property_type(name, type);
}

auto object::get_property(const QString& name) const -> const property&
{
  return m_properties.get_property(name);
}

auto object::get_property(const QString& name) -> property&
{
  return m_properties.get_property(name);
}

auto object::has_property(const QString& name) const -> bool
{
  return m_properties.has_property(name);
}

auto object::property_count() const noexcept -> int
{
  return m_properties.property_count();
}

auto object::properties() const -> const property_map&
{
  return m_properties.properties();
}

}  // namespace tactile::core

#include "object_layer.hpp"

#include <utility>  // move

#include "tactile_qstring.hpp"

namespace tactile::core {

object_layer::object_layer() : m_delegate{layer_type::object_layer}
{
  m_delegate.set_name(TACTILE_QSTRING(u"Object layer"));
}

void object_layer::add_property(const QString& name, const property_type type)
{
  m_delegate.add_property(name, type);
}

void object_layer::add_property(const QString& name, const property& property)
{
  m_delegate.add_property(name, property);
}

void object_layer::remove_property(const QString& name)
{
  m_delegate.remove_property(name);
}

void object_layer::rename_property(const QString& oldName,
                                   const QString& newName)
{
  m_delegate.rename_property(oldName, newName);
}

void object_layer::set_property(const QString& name, const property& property)
{
  m_delegate.set_property(name, property);
}

void object_layer::change_property_type(const QString& name,
                                        const core::property_type type)
{
  m_delegate.change_property_type(name, type);
}

auto object_layer::get_property(const QString& name) const -> const property&
{
  return m_delegate.get_property(name);
}

auto object_layer::get_property(const QString& name) -> property&
{
  return m_delegate.get_property(name);
}

auto object_layer::has_property(const QString& name) const -> bool
{
  return m_delegate.has_property(name);
}

auto object_layer::property_count() const noexcept -> int
{
  return m_delegate.property_count();
}

auto object_layer::properties() const -> const property_map&
{
  return m_delegate.properties();
}

void object_layer::SetVisible(const bool visible) noexcept
{
  m_delegate.set_visible(visible);
}

void object_layer::SetOpacity(const double opacity)
{
  m_delegate.set_opacity(opacity);
}

void object_layer::SetName(QString name)
{
  m_delegate.set_name(std::move(name));
}

auto object_layer::Type() const -> layer_type
{
  return m_delegate.type();
}

auto object_layer::IsVisible() const noexcept -> bool
{
  return m_delegate.visible();
}

auto object_layer::Opacity() const noexcept -> double
{
  return m_delegate.opacity();
}

auto object_layer::Name() const -> const QString&
{
  return m_delegate.name();
}

auto object_layer::Clone() const -> shared<ILayer>
{
  return std::make_shared<object_layer>(*this);
}

void object_layer::add_object(const object_id id, object obj)
{
  Q_ASSERT(!has_object(id));
  m_objects.emplace(id, std::move(obj));
}

void object_layer::add_point(const object_id id, const double x, const double y)
{
  Q_ASSERT(!has_object(id));
  auto& [key, point] = m_objects.emplace(id, object_type::point);
  point.set_x(x);
  point.set_y(y);
}

void object_layer::add_rectangle(const object_id id,
                                 const double x,
                                 const double y,
                                 const double width,
                                 const double height)
{
  Q_ASSERT(!has_object(id));
  auto& [key, rect] = m_objects.emplace(id, object_type::rectangle);
  rect.set_x(x);
  rect.set_y(y);
  rect.set_width(width);
  rect.set_height(height);
}

void object_layer::remove_object(const object_id id)
{
  Q_ASSERT(has_object(id));
  m_objects.erase(id);
}

auto object_layer::has_object(const object_id id) const -> bool
{
  return m_objects.contains(id);
}

auto object_layer::get_object(const object_id id) -> object&
{
  return m_objects.at(id);
}

auto object_layer::get_object(const object_id id) const -> const object&
{
  return m_objects.at(id);
}

auto object_layer::object_count() const noexcept -> int
{
  return static_cast<int>(m_objects.size());
}

}  // namespace tactile::core

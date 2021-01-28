#include "object_layer.hpp"

#include <type_traits>  // is_copy_constructible_v, is_copy_assignable_v, ...
#include <utility>      // move

#include "point_object.hpp"
#include "tactile_qstring.hpp"

namespace tactile::core {

static_assert(std::is_nothrow_move_constructible_v<object_layer>);
static_assert(std::is_nothrow_move_assignable_v<object_layer>);
static_assert(std::is_copy_constructible_v<object_layer>);
static_assert(std::is_copy_assignable_v<object_layer>);

object_layer::object_layer() : m_delegate{layer_type::object_layer}
{
  m_delegate.set_name(TACTILE_QSTRING(u"Object layer"));
}

object_layer::object_layer(const object_layer& other)
    : m_delegate{other.m_delegate}
{
  copy_objects(other);
}

auto object_layer::operator=(const object_layer& other) -> object_layer&
{
  if (this != &other) {
    m_delegate = other.m_delegate;
    copy_objects(other);
  }
  return *this;
}

void object_layer::copy_objects(const object_layer& other)
{
  m_objects.reserve(other.m_objects.size());
  for (const auto& [id, object] : other.m_objects) {
    m_objects.emplace(id, object->clone());
  }
}

void object_layer::add_property(const QString& name, const property::type type)
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

auto object_layer::get_property(const QString& name) const -> const property&
{
  return m_delegate.get_property(name);
}

auto object_layer::get_property(const QString& name) -> property&
{
  return m_delegate.get_property(name);
}

auto object_layer::property_count() const noexcept -> int
{
  return m_delegate.property_count();
}

auto object_layer::properties() const -> const property_map&
{
  return m_delegate.properties();
}

void object_layer::set_visible(const bool visible) noexcept
{
  m_delegate.set_visible(visible);
}

void object_layer::set_opacity(const double opacity)
{
  m_delegate.set_opacity(opacity);
}

void object_layer::set_name(QString name)
{
  m_delegate.set_name(std::move(name));
}

auto object_layer::type() const -> layer_type
{
  return m_delegate.type();
}

auto object_layer::visible() const noexcept -> bool
{
  return m_delegate.visible();
}

auto object_layer::opacity() const noexcept -> double
{
  return m_delegate.opacity();
}

auto object_layer::name() const -> const QString&
{
  return m_delegate.name();
}

auto object_layer::clone() const -> shared<layer>
{
  return std::make_shared<object_layer>(*this);
}
}

}  // namespace tactile::core

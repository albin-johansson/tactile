#include "object_layer.hpp"

#include <utility>  // move

namespace tactile::core {

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

}  // namespace tactile::core

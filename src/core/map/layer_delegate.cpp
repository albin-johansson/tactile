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

}  // namespace tactile::core

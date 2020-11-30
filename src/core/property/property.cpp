#include "property.hpp"

namespace tactile::core {

void property::reset()
{
  m_value.emplace<std::monostate>();
}

auto property::has_value() const noexcept -> bool
{
  return !std::holds_alternative<std::monostate>(m_value);
}

auto property::get_type() const -> maybe<property::type>
{
  if (is<int>()) {
    return integer;

  } else if (is<double>()) {
    return floating;

  } else if (is<QString>()) {
    return string;

  } else if (is<bool>()) {
    return boolean;

  } else {
    return std::nullopt;
  }
}

auto property::is_string() const noexcept -> bool
{
  return get_type() == string;
}

auto property::is_integer() const noexcept -> bool
{
  return get_type() == integer;
}

auto property::is_floating() const noexcept -> bool
{
  return get_type() == floating;
}

auto property::is_boolean() const noexcept -> bool
{
  return get_type() == boolean;
}

}  // namespace tactile::core

#include "tactile_error.hpp"

#include <utility>

namespace tactile {

tactile_error::tactile_error(std::string what)
{
  if (!what.empty()) {
    m_what = std::move(what);
  }
}

auto tactile_error::what() const noexcept -> czstring
{
  return m_what.c_str();
}

}  // namespace tactile

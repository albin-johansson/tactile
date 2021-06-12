#include "string_utils.hpp"

namespace tactile {

auto ToInteger(const QString& string) -> Maybe<int>
{
  bool ok{};
  if (const auto value = string.toInt(&ok); ok)
  {
    return value;
  }
  else
  {
    return nothing;
  }
}

}  // namespace tactile

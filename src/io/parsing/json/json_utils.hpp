#pragma once

#include "aliases/czstring.hpp"
#include "aliases/json.hpp"
#include "aliases/maybe.hpp"

namespace Tactile::IO {

template <typename T>
[[nodiscard]] auto GetValue(const JSON& json, const czstring key) -> Maybe<T>
{
  if (const auto it = json.find(key); it != json.end())
  {
    return it->get<T>();
  }
  else
  {
    return nothing;
  }
}

}  // namespace Tactile::IO

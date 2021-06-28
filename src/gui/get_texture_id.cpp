#include "get_texture_id.hpp"

#include <cstdint>  // uintptr_t

namespace tactile {

auto GetTextureID(const uint texture) -> ImTextureID
{
  const auto value = static_cast<std::uintptr_t>(texture);
  return reinterpret_cast<void*>(value);  // NOLINT
}

}  // namespace tactile

#include "texture_utils.hpp"

#include <cstdint>  // uintptr_t

namespace Tactile {

auto ToTextureID(const uint texture) -> ImTextureID
{
  const auto value = static_cast<std::uintptr_t>(texture);
  return reinterpret_cast<ImTextureID>(value);  // NOLINT
}

}  // namespace Tactile

#include "textures.hpp"

#include <cstdint>  // uintptr_t

namespace tactile {

auto to_texture_id(const uint texture) noexcept -> ImTextureID
{
  const auto value = static_cast<std::uintptr_t>(texture);
  return reinterpret_cast<ImTextureID>(value);  // NOLINT
}

}  // namespace tactile

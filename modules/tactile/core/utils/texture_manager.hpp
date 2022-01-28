#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "core/components/texture.hpp"
#include "tactile_def.hpp"

namespace tactile {

class texture_manager final {
 public:
  TACTILE_DEFAULT_COPY(texture_manager)
  TACTILE_DEFAULT_MOVE(texture_manager)

  texture_manager() = default;

  ~texture_manager();

  [[nodiscard]] auto load(const std::filesystem::path& path) -> Maybe<comp::texture>;

 private:
  std::vector<uint> mTextures;
};

}  // namespace tactile

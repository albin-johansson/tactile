#include "directories.hpp"

#include <centurion.hpp>  // preferred_path

namespace tactile {

auto persistent_file_dir() -> const std::filesystem::path&
{
  static const std::filesystem::path path{
      cen::preferred_path("albin-johansson", "tactile").copy()};
  return path;
}

}  // namespace tactile

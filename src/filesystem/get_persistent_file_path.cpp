#include "get_persistent_file_path.hpp"

#include <centurion.hpp>  // preferred_path

namespace tactile {

auto GetPersistentFilePath() -> const std::filesystem::path&
{
  static const std::filesystem::path path{
      cen::preferred_path("albin-johansson", "tactile").copy()};
  return path;
};

}  // namespace tactile

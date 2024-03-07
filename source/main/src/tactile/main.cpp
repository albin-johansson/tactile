#include <cstdlib>
#include <filesystem>

auto main(const int, char*[]) -> int
{
  const auto binary_dir = std::filesystem::current_path();
  return EXIT_SUCCESS;
}

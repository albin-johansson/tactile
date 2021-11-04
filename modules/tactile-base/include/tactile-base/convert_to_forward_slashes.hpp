#pragma once

#include <algorithm>   // replace
#include <filesystem>  // path
#include <string>      // string

namespace Tactile {

[[nodiscard]] inline auto ConvertToForwardSlashes(const std::filesystem::path& path)
    -> std::string
{
  /* Here we make sure that the file path is portable, by using forward slashes that
     can be understood by pretty much all operating systems that we care about. */
  auto str = path.string();
  std::replace(str.begin(), str.end(), '\\', '/');
  return str;
}

}  // namespace Tactile
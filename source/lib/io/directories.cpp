// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "directories.hpp"

#include <cstdlib>  // system

#include <centurion/filesystem.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

void open_directory(const Path& dir)
{
  if (fs::is_directory(dir)) {
    static const auto path = get_persistent_file_dir().string();
    if constexpr (kOnMacos) {
      static const auto cmd = fmt::format("open \"{}\"", path);
      std::system(cmd.c_str());
    }
    else if constexpr (kOnWindows) {
      static const auto cmd = fmt::format("explorer \"{}\"", path);
      std::system(cmd.c_str());
    }
    else if constexpr (kOnLinux) {
      static const auto cmd = fmt::format("xdg-open \"{}\"", path);
      std::system(cmd.c_str());
    }
    else {
      spdlog::warn("Cannot open file explorer on this platform!");
    }
  }
  else {
    throw Exception {"Not a directory!"};
  }
}

auto find_resource(const char* resource) -> Path
{
  TACTILE_ASSERT(resource);
  static const Path dir {cen::base_path().copy()};
  return dir / resource;
}

auto get_widget_ini_path() -> const Path&
{
  static const auto ini = fs::absolute(find_resource("imgui.ini"));
  return ini;
}

auto get_persistent_file_dir() -> const Path&
{
  static const Path path {cen::preferred_path("albin-johansson", "tactile").copy()};
  return path;
}

}  // namespace tactile

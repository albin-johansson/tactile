#include "file_dialog.hpp"

#include <tinyfiledialogs.h>

#include "misc/assert.hpp"

namespace tactile {
namespace {

constexpr int _num_map_patterns = 5;
constexpr const char* _map_patterns[] = {"*.yaml", "*.yml", "*.json", "*.tmx", "*.xml"};
constexpr const char* _map_pattern_descriptor = "Map files";

constexpr int _num_image_patterns = 3;
constexpr const char* _image_patterns[] = {"*.png", "*.jpg", "*.jpeg"};
constexpr const char* _image_pattern_descriptor = "Image files";

}  // namespace

file_dialog::file_dialog(const char* path) : mPath{path} {}

auto file_dialog::is_okay() const noexcept -> bool
{
  return mPath != nullptr;
}

auto file_dialog::path() const -> std::filesystem::path
{
  TACTILE_ASSERT(is_okay());
  return {mPath};
}

auto file_dialog::open_file() -> file_dialog
{
  return {tinyfd_openFileDialog("Open File", nullptr, 0, nullptr, "Any file", 0)};
}

auto file_dialog::open_map() -> file_dialog
{
  return {tinyfd_openFileDialog("Open Map",
                                nullptr,
                                _num_map_patterns,
                                _map_patterns,
                                _map_pattern_descriptor,
                                0)};
}

auto file_dialog::open_image() -> file_dialog
{
  return {tinyfd_openFileDialog("Open Image",
                                nullptr,
                                _num_image_patterns,
                                _image_patterns,
                                _image_pattern_descriptor,
                                0)};
}

auto file_dialog::save_map() -> file_dialog
{
  return {tinyfd_saveFileDialog("Save Map",
                                nullptr,
                                _num_map_patterns,
                                _map_patterns,
                                _map_pattern_descriptor)};
}

auto file_dialog::save_image() -> file_dialog
{
  return {tinyfd_saveFileDialog("Save Image",
                                nullptr,
                                _num_image_patterns,
                                _image_patterns,
                                _image_pattern_descriptor)};
}

}  // namespace tactile

#include "file_dialog.hpp"

#include <cstdlib>  // free

#include "assert.hpp"

namespace tactile {
namespace {

constexpr const nfdchar_t* gMapFileFilter = "yaml,yml,json,tmx,xml";
constexpr const nfdchar_t* gImageFileFilter = "png,jpg";

}  // namespace

file_dialog::file_dialog(const nfdresult_t result, nfdchar_t* path)
    : mResult{result}
    , mPath{path}
{}

file_dialog::~file_dialog() noexcept
{
  if (mResult == NFD_OKAY) {
    free(mPath);
  }
}

auto file_dialog::is_okay() const noexcept -> bool
{
  return mResult == NFD_OKAY;
}

auto file_dialog::was_canceled() const noexcept -> bool
{
  return mResult == NFD_CANCEL;
}

auto file_dialog::path() const -> std::filesystem::path
{
  TACTILE_ASSERT(is_okay());
  return {mPath};
}

auto file_dialog::open_file(const nfdchar_t* filter, const nfdchar_t* defaultPath)
    -> file_dialog
{
  nfdchar_t* path{};
  return {NFD_OpenDialog(filter, defaultPath, &path), path};
}

auto file_dialog::open_map() -> file_dialog
{
  return open_file(gMapFileFilter);
}

auto file_dialog::open_image() -> file_dialog
{
  return open_file(gImageFileFilter);
}

auto file_dialog::save_file(const nfdchar_t* filter, const nfdchar_t* defaultPath)
    -> file_dialog
{
  nfdchar_t* path{};
  return {NFD_SaveDialog(filter, defaultPath, &path), path};
}

auto file_dialog::save_map() -> file_dialog
{
  return save_file(gMapFileFilter);
}

auto file_dialog::save_image() -> file_dialog
{
  return save_file(gImageFileFilter);
}

}  // namespace tactile

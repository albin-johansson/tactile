#include "file_dialog.hpp"

#include <cstdlib>  // free

#include "assert.hpp"

namespace tactile {
namespace {

constexpr const nfdchar_t* gMapFileFilter = "yaml,yml,json,tmx,xml";
constexpr const nfdchar_t* gImageFileFilter = "png,jpg";

}  // namespace

FileDialog::FileDialog(const nfdresult_t result, nfdchar_t* path)
    : mResult{result}
    , mPath{path}
{}

FileDialog::~FileDialog() noexcept
{
  if (mResult == NFD_OKAY) {
    free(mPath);
  }
}

auto FileDialog::IsOkay() const noexcept -> bool
{
  return mResult == NFD_OKAY;
}

auto FileDialog::WasCanceled() const noexcept -> bool
{
  return mResult == NFD_CANCEL;
}

auto FileDialog::GetPath() const -> std::filesystem::path
{
  TACTILE_ASSERT(IsOkay());
  return {mPath};
}

auto FileDialog::OpenFile(const nfdchar_t* filter, const nfdchar_t* defaultPath)
    -> FileDialog
{
  nfdchar_t* path{};
  return {NFD_OpenDialog(filter, defaultPath, &path), path};
}

auto FileDialog::OpenMap() -> FileDialog
{
  return OpenFile(gMapFileFilter);
}

auto FileDialog::OpenImage() -> FileDialog
{
  return OpenFile(gImageFileFilter);
}

auto FileDialog::SaveFile(const nfdchar_t* filter, const nfdchar_t* defaultPath)
    -> FileDialog
{
  nfdchar_t* path{};
  return {NFD_SaveDialog(filter, defaultPath, &path), path};
}

auto FileDialog::SaveMap() -> FileDialog
{
  return SaveFile(gMapFileFilter);
}

auto FileDialog::SaveImage() -> FileDialog
{
  return SaveFile(gImageFileFilter);
}

}  // namespace tactile

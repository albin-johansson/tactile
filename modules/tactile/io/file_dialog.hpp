#pragma once

#include <filesystem>  // path

#include <nfd.h>
#include <tactile_def.hpp>

namespace tactile {

class FileDialog final {
 public:
  TACTILE_DELETE_COPY(FileDialog)
  TACTILE_DELETE_MOVE(FileDialog)

  ~FileDialog() noexcept;

  [[nodiscard]] static auto OpenFile(const nfdchar_t* filter = nullptr,
                                     const nfdchar_t* defaultPath = nullptr)
      -> FileDialog;

  [[nodiscard]] static auto OpenMap() -> FileDialog;

  [[nodiscard]] static auto OpenImage() -> FileDialog;

  [[nodiscard]] static auto SaveFile(const nfdchar_t* filter = nullptr,
                                     const nfdchar_t* defaultPath = nullptr)
      -> FileDialog;

  [[nodiscard]] static auto SaveMap() -> FileDialog;

  [[nodiscard]] static auto SaveImage() -> FileDialog;

  [[nodiscard]] auto IsOkay() const noexcept -> bool;

  [[nodiscard]] auto WasCanceled() const noexcept -> bool;

  [[nodiscard]] auto GetPath() const -> std::filesystem::path;

 private:
  nfdresult_t mResult{};
  nfdchar_t* mPath{};

  FileDialog(nfdresult_t result, nfdchar_t* path);
};

}  // namespace tactile

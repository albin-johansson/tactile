// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/basic/macros.hpp"
#include "tactile/core/basic/result.hpp"
#include "tactile/core/basic/smart_ptr.hpp"

#if TACTILE_OS_WINDOWS
  #define TACTILE_DYLIB_EXT ".dll"
#elif TACTILE_OS_APPLE
  #define TACTILE_DYLIB_EXT ".dylib"
#else
  #define TACTILE_DYLIB_EXT ".so"
#endif

namespace tactile::editor {

/// Represents a dynamically loaded library (DLL)
///
/// Dynamic library files typically use file extensions such as `.so` on Linux, `.so`
/// or `.dylib` on macOS, and `.dll` on Windows. This API can be used to obtain
/// pointers to symbols with C linkage in such files at runtime.
class DynamicLibrary final
{
 public:
  TACTILE_PIMPL_CLASS(DynamicLibrary);

  /// Attempts to open a dynamic library file.
  [[nodiscard]]
  static auto open(const char* path) -> Result<DynamicLibrary>;

  /// Attempts to find a pointer to a function in the library.
  ///
  /// This function should only be used to obtain pointers to functions with C
  /// linkage to avoid running into C++ ABI issues. The actual functions may of
  /// course be implemented in C++.
  [[nodiscard]]
  auto find_function(const char* name) const -> void*;

 private:
  class Impl;
  Unique<Impl> m_impl;

  DynamicLibrary();
};

}  // namespace tactile::editor

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path
#include <memory>      // unique_ptr

#include "tactile/base/prelude.hpp"

#if TACTILE_OS_WINDOWS
  #define TACTILE_DLL_EXT ".dll"
#elif TACTILE_OS_APPLE
  #define TACTILE_DLL_EXT ".dylib"
#else
  #define TACTILE_DLL_EXT ".so"
#endif

namespace tactile::core {

/**
 * Interface for dynamically loaded libraries.
 *
 * \details
 * Dynamic library files typically use file extensions such as \c .so on
 * Linux, \c .so or \c .dylib on macOS, and \c .dll on Windows. This interface
 * can be used to obtain pointers to symbols with C linkage in such files at
 * runtime.
 *
 * \see load_library(const std::filesystem::path&)
 */
class IDynamicLibrary
{
 public:
  TACTILE_INTERFACE_CLASS(IDynamicLibrary);

  /**
   * Attempts to find a pointer to a symbol in the library.
   *
   * \details
   * This function should only be used to obtain pointers to entities with C
   * linkage to avoid running into C++ ABI issues. The actual functions may of
   * course be implemented in C++.
   *
   * \param name The symbol name.
   *
   * \return
   * A pointer to the symbol if successful; a null pointer otherwise.
   */
  [[nodiscard]]
  virtual auto find_symbol(const char* name) const -> void* = 0;
};

/**
 * Attempts to load a dynamic library.
 *
 * \details
 * This function makes use of the `dlopen` and `LoadModuleA` APIs on Linux/macOS
 * and Windows, respectively. For other platforms, this function simply returns
 * a null pointer.
 *
 * \param path The file path to the dynamic library.
 *
 * \return
 * The loaded library if successful; a null pointer otherwise.
 */
[[nodiscard]]
auto load_library(const std::filesystem::path& path) -> std::unique_ptr<IDynamicLibrary>;

/**
 * Attempts to find a symbol in a dynamic library.
 *
 * \pre The actual symbol type must match the provided type.
 *
 * \tparam T The type of the symbol.
 *
 * \param lib  The associated dynamic library.
 * \param name The name of the symbol to load.
 *
 * \return
 * A pointer to the symbol if successful; a null pointer otherwise.
 */
template <typename T>
[[nodiscard]] auto find_symbol(const IDynamicLibrary& lib, const char* name) -> T*
{
  return reinterpret_cast<T*>(lib.find_symbol(name));
}

}  // namespace tactile::core

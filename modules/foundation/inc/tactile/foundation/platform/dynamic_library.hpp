// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Simple interface for dynamically loaded libraries.
 *
 * \details
 *    Dynamic library files typically use file extensions such as `.so` on Linux,
 *    `.so` or `.dylib` on macOS, and `.dll` on Windows. Use this interface to
 *    obtain pointers to C functions provided in such files at runtime.
 *
 * \see load_library(const char*)
 */
class IDynamicLibrary {
 public:
  TACTILE_INTERFACE_CLASS(IDynamicLibrary);

  /**
   * Returns a pointer to a symbol in the library.
   *
   * \details
   *    This function should only be used to obtain pointers to entities with C
   *    linkage to avoid running into C++ ABI issues. The actual functions may
   *    of course be implemented in C++.
   *
   * \param name The symbol name.
   *
   * \return
   *    A pointer to the found symbol; a null pointer otherwise.
   */
  [[nodiscard]]
  virtual auto get_symbol(const char* name) const -> void* = 0;

  /**
   * Returns the file path to the dynamic library binary.
   *
   * \return
   *    A file path.
   */
  [[nodiscard]]
  virtual auto get_path() const -> const FilePath& = 0;
};

/**
 * Convenience function for loading symbols from a dynamic library.
 *
 * \pre The symbol must have the specified type.
 *
 * \tparam T The type of the symbol.
 *
 * \param library The source dynamic library.
 * \param name    The name of the symbol to load.
 *
 * \return
 *    A pointer to the symbol.
 */
template <typename T>
[[nodiscard]] auto get_symbol(const IDynamicLibrary& library, const char* name) -> T*
{
  return reinterpret_cast<T*>(library.get_symbol(name));
}

/**
 * Indicates whether a file is likely to be a dynamic library.
 *
 * \note
 *    This function merely provides a course heuristic useful for excluding files
 *    that are unlikely to be dynamic libraries. It works by simply checking the file
 *    extension of the specified file and compares the extension to established
 *    dynamic library file extensions for the current platform.
 *
 * \param path The path to the file to check.
 *
 * \return
 *    True if the file could be a dynamic library; false otherwise.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto is_dll(const FilePath& path) -> bool;

/**
 * Attempts to load a dynamic library.
 *
 * \details
 *    This function makes use of the `dlopen` and `LoadModuleA` APIs on Linux/macOS
 *    and Windows, respectively. For other platforms, this function simply returns a
 *    null pointer.
 *
 * \param path The file path to the dynamic library.
 *
 * \return
 *    The loaded library, or a null pointer on failure.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto load_library(const FilePath& path) -> Unique<IDynamicLibrary>;

}  // namespace tactile

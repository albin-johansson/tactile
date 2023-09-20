// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \interface IDynamicLibrary
 *
 * \brief Simple interface for dynamically loaded libraries.
 *
 * \details Dynamic library files typically use file extensions such as `.so` on Linux,
 *          `.so` or `.dylib` on macOS, and `.dll` on Windows. Use this interface to
 *          obtain pointers to C functions provided in such files at runtime.
 *
 * \see load_library(const char*)
 */
class TACTILE_CORE_API IDynamicLibrary {
 public:
  TACTILE_INTERFACE_CLASS(IDynamicLibrary);

  /**
   * \brief Returns a pointer to a symbol in the library.
   *
   * \details This function should only be used to obtain pointers to entities with C
   *          linkage to avoid running into C++ ABI issues. The actual functions may
   *          of course be implemented in C++.
   *
   * \param name the symbol name.
   *
   * \return a pointer to the found symbol; a null pointer otherwise.
   */
  [[nodiscard]] virtual auto get_symbol(const char* name) const -> void* = 0;
};

template <typename FnType>
[[nodiscard]] auto get_symbol(const IDynamicLibrary& library, const char* name) -> FnType
{
  return reinterpret_cast<FnType>(library.get_symbol(name));
}

/**
 * \brief Attempts to load a dynamic library.
 *
 * \details This function makes use of the `dlopen` and `LoadModuleA` APIs on Linux/macOS
 *          and Windows, respectively. For other platforms, this function simply returns a
 *          null pointer.
 *
 * \param library_path the file path to the dynamic library.
 *
 * \return the loaded library, or a null pointer on failure.
 */
[[nodiscard]] TACTILE_CORE_API auto load_library(const fs::Path& library_path)
    -> Unique<IDynamicLibrary>;

}  // namespace tactile

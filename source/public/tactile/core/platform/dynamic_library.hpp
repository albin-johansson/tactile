/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/common/prelude.hpp"
#include "tactile/core/containers/smart_ptr.hpp"

namespace tactile {

/** Generic interface for dynamic libraries, such as `.so` and `.dll` files. */
class TACTILE_CORE_API IDynamicLibrary {
 public:
  virtual ~IDynamicLibrary() noexcept = default;

  /** Attempts to find a symbol with a given name in the library. */
  [[nodiscard]] virtual auto get_symbol(const char* name) const -> void* = 0;
};

/** Attempts to load a dynamic library, returning null on failure. */
[[nodiscard]] TACTILE_CORE_API auto load_dynamic_library(const char* path)
    -> UniquePtr<IDynamicLibrary>;

}  // namespace tactile
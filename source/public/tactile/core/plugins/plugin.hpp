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
#include "tactile/core/platform/dynamic_library.hpp"
#include "tactile/core/rhi/graphics_api.hpp"
#include "tactile/core/rhi/render_device.hpp"

namespace tactile {

using PluginIDFn = const char* (*) ();
using RHIGetAPIFn = GraphicsAPI (*)();
using RHICreateDeviceFn = IRenderDevice* (*) ();
using RHIDeleteDeviceFn = void (*)(IRenderDevice*);

using UniqueRenderDevice = UniquePtr<IRenderDevice, RHIDeleteDeviceFn>;

/** Represents a plugin that has been dynamically loaded. */
struct TACTILE_CORE_API Plugin final {
  UniquePtr<IDynamicLibrary> library;      ///< The dynamic library handle.
  PluginIDFn id {};                        ///< Guaranteed to be available.
  RHIGetAPIFn rhi_get_api {};              ///< Only available for RHI plugins.
  RHICreateDeviceFn rhi_create_device {};  ///< Only available for RHI plugins.
  RHIDeleteDeviceFn rhi_delete_device {};  ///< Only available for RHI plugins.

  /** Indicates whether the plugin is an RHI implementation. */
  [[nodiscard]] auto is_rhi() const -> bool;
};

}  // namespace tactile

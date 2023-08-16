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

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/rhi/graphics_api.hpp"
#include "tactile/core/rhi/render_device.hpp"

#ifdef TACTILE_BUILDING_OPENGL_RHI
  #define TACTILE_OPENGL_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_OPENGL_API TACTILE_DLL_IMPORT
#endif  // TACTILE_BUILDING_OPENGL_RHI

namespace tactile {

extern "C" TACTILE_OPENGL_API auto tactile_plugin_id() -> const char*;

extern "C" TACTILE_OPENGL_API auto tactile_rhi_get_api() -> GraphicsAPI;

extern "C" TACTILE_OPENGL_API auto tactile_rhi_create_device() -> IRenderDevice*;

extern "C" TACTILE_OPENGL_API void tactile_rhi_delete_device(IRenderDevice* device);

}  // namespace tactile

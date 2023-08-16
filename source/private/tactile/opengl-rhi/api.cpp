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

#include "tactile/opengl-rhi/api.hpp"

#include "tactile/opengl-rhi/gl_render_device.hpp"

namespace tactile {

auto tactile_plugin_id() -> const char*
{
  return "tactile.rhi.opengl";
}

auto tactile_rhi_get_api() -> GraphicsAPI
{
  return GraphicsAPI::OpenGL;
}

auto tactile_rhi_create_device() -> IRenderDevice*
{
  return new GLRenderDevice {};
}

void tactile_rhi_delete_device(IRenderDevice* device)
{
  delete device;
}

}  // namespace tactile
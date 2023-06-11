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

#include "vk_backend.hpp"

#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>


namespace tactile {

VulkanBackend::VulkanBackend(SDL_Window* window)
{
}

VulkanBackend::~VulkanBackend() noexcept
{
}

void VulkanBackend::process_event(const SDL_Event* event)
{
  ImGui_ImplSDL2_ProcessEvent(event);
}

auto VulkanBackend::new_frame() -> Result
{
  return failure;
}

void VulkanBackend::end_frame() {}

auto VulkanBackend::can_reload_fonts() const -> bool
{
  return false;
}

}  // namespace tactile

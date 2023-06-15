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

#include "backend/vk/vk_context.hpp"
#include "common/debug/panic.hpp"

namespace tactile {

VulkanBackend::VulkanBackend(SDL_Window* window)
    : mInstance {vk::create_instance(window)},
      mSurface {vk::create_surface(mInstance.get(), window)},
      mGPU {vk::get_suitable_gpu(mInstance.get(), mSurface.get())},
      mDevice {vk::create_device(mGPU, mSurface.get())},
      mAllocator {vk::create_allocator(mInstance.get(), mGPU, mDevice.get())}
{
  vk::set_global_instance(mInstance.get());
  vk::set_global_device(mDevice.get());
  vk::set_global_allocator(mAllocator.get());

  {
    const auto indices = vk::get_queue_family_indices(mGPU, mSurface.get());
    vkGetDeviceQueue(mDevice.get(), indices.graphics_family.value(), 0, &mGraphicsQueue);
    vkGetDeviceQueue(mDevice.get(), indices.present_family.value(), 0, &mPresentQueue);
  }

  if (!ImGui_ImplSDL2_InitForVulkan(window)) {
    throw TactileError {"Could not initialize ImGui SDL2 backend"};
  }

  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = mInstance.get();
  VkRenderPass render_pass = VK_NULL_HANDLE;

  if (!ImGui_ImplVulkan_Init(&init_info, render_pass)) {
    ImGui_ImplSDL2_Shutdown();
    throw TactileError {"Could not initialize ImGui Vulkan backend"};
  }
}

VulkanBackend::~VulkanBackend() noexcept
{
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplSDL2_Shutdown();
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

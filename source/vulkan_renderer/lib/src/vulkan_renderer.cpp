// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_renderer.hpp"

#include <cstdint>       // uint32_t, uint64_t
#include <limits>        // numeric_limits
#include <system_error>  // make_error_code, errc
#include <tuple>         // ignore
#include <utility>       // move

#include <SDL_events.h>
#include <SDL_vulkan.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>

#include "tactile/base/io/file_io.hpp"
#include "tactile/base/render/window.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/runtime/runtime.hpp"
#include "tactile/vulkan_renderer/vulkan_buffer.hpp"
#include "tactile/vulkan_renderer/vulkan_physical_device.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {
namespace {

inline constexpr std::size_t _max_frame_in_flight_count {2};

void _init_frames(std::vector<VulkanFrame>& frames,
                  VkDevice device,
                  VkCommandPool command_pool)
{
  frames.reserve(_max_frame_in_flight_count);
  for (std::size_t i = 0; i < _max_frame_in_flight_count; ++i) {
    VulkanFrame frame {};

    frame.command_buffer = create_vulkan_command_buffer(device, command_pool).value();
    frame.in_flight_fence = create_vulkan_fence(device, VK_FENCE_CREATE_SIGNALED_BIT).value();
    frame.image_available_semaphore = create_vulkan_semaphore(device).value();
    frame.render_finished_semaphore = create_vulkan_semaphore(device).value();

    frames.push_back(std::move(frame));
  }
}

}  // namespace

VulkanRenderer::VulkanRenderer(IWindow* window)
  : m_window {window},
    m_instance {create_vulkan_instance(*m_window).value()},
    m_surface {create_vulkan_surface(m_instance.get(), *m_window).value()},
    m_physical_device {select_physical_device(m_instance.get(), m_surface.handle).value()},
    m_device {create_vulkan_device(m_physical_device, m_surface.handle).value()},
    m_allocator {
      create_vulkan_allocator(m_instance.get(), m_physical_device, m_device.handle).value()},
    m_swapchain {create_vulkan_swapchain(m_surface.handle,
                                         m_physical_device,
                                         m_device.handle,
                                         m_allocator.handle)
                     .value()},
    m_graphics_command_pool {
      create_vulkan_command_pool(m_device.handle,
                                 m_device.graphics_queue_family,
                                 VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
          .value()},
    m_sampler {create_vulkan_sampler(m_device.handle).value()},
    m_imgui_descriptor_pool {create_vulkan_imgui_descriptor_pool(m_device.handle).value()},
    m_vkCmdBeginRendering {reinterpret_cast<PFN_vkCmdBeginRenderingKHR>(
        vkGetInstanceProcAddr(m_instance.get(), "vkCmdBeginRenderingKHR"))},
    m_vkCmdEndRendering {reinterpret_cast<PFN_vkCmdEndRenderingKHR>(
        vkGetInstanceProcAddr(m_instance.get(), "vkCmdEndRenderingKHR"))}
{
  vkGetDeviceQueue(m_device.handle, m_device.graphics_queue_family, 0, &m_graphics_queue);
  vkGetDeviceQueue(m_device.handle, m_device.presentation_queue_family, 0, &m_present_queue);

  if (m_graphics_queue == VK_NULL_HANDLE || m_present_queue == VK_NULL_HANDLE) {
    log(LogLevel::kError, "Could not get Vulkan device queues");
    throw std::runtime_error {"Could not get Vulkan device queues"};
  }

  _init_frames(m_frames, m_device.handle, m_graphics_command_pool.handle);

  const VkPipelineRenderingCreateInfoKHR pipeline_rendering_info {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
    .pNext = nullptr,
    .viewMask = 0,
    .colorAttachmentCount = 1,
    .pColorAttachmentFormats = &m_swapchain.params.image_format,
    .depthAttachmentFormat = m_swapchain.params.depth_format,
    .stencilAttachmentFormat = VK_FORMAT_UNDEFINED,
  };

  ImGui_ImplVulkan_InitInfo imgui_vulkan_info {
    .Instance = m_instance.get(),
    .PhysicalDevice = m_physical_device,
    .Device = m_device.handle,
    .QueueFamily = m_device.graphics_queue_family,
    .Queue = m_graphics_queue,
    .DescriptorPool = m_imgui_descriptor_pool.handle,
    .RenderPass = nullptr,
    .MinImageCount = m_swapchain.params.min_image_count,
    .ImageCount = static_cast<std::uint32_t>(m_swapchain.images.size()),
    .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
    .PipelineCache = nullptr,
    .Subpass = 0,
    .UseDynamicRendering = true,
    .PipelineRenderingCreateInfo = pipeline_rendering_info,
    .Allocator = nullptr,
    .CheckVkResultFn = nullptr,
    .MinAllocationSize = 1024u * 1024u,
  };

  m_imgui_context = VulkanImGuiContext::init(*m_window, imgui_vulkan_info).value();
}

VulkanRenderer::~VulkanRenderer() noexcept
{
  // Wait for the GPU to finish working so that we don't destroy any active resources.
  std::ignore = vkDeviceWaitIdle(m_device.handle);
}

auto VulkanRenderer::begin_frame() -> bool
{
  const auto& frame = m_frames.at(m_frame_index);

  int width {};
  int height {};
  SDL_GetWindowSize(m_window->get_handle(), &width, &height);

  int width_pixels {};
  int height_pixels {};
  SDL_GetWindowSizeInPixels(m_window->get_handle(), &width_pixels, &height_pixels);

  const auto x_scale = static_cast<float>(width_pixels) / static_cast<float>(width);
  const auto y_scale = static_cast<float>(height_pixels) / static_cast<float>(height);

  auto& io = ImGui::GetIO();
  io.DisplaySize.x = width;
  io.DisplaySize.y = height;
  io.DisplayFramebufferScale = ImVec2 {x_scale, y_scale};

  // Wait until the GPU has finished executing previously submitted commands.
  std::ignore = vkWaitForFences(m_device.handle,
                                1,
                                &frame.in_flight_fence.handle,
                                VK_TRUE,
                                std::numeric_limits<std::uint64_t>::max());
  std::ignore = vkResetFences(m_device.handle, 1, &frame.in_flight_fence.handle);

  const auto acquire_result = vkAcquireNextImageKHR(m_device.handle,
                                                    m_swapchain.handle,
                                                    std::numeric_limits<std::uint64_t>::max(),
                                                    frame.image_available_semaphore.handle,
                                                    nullptr,
                                                    &m_swapchain.image_index);

  if (acquire_result != VK_SUCCESS && acquire_result != VK_SUBOPTIMAL_KHR) {
    m_frame_index = (m_frame_index + 1) % m_frames.size();
    return false;
  }

  ImGui::NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui_ImplVulkan_NewFrame();

  return true;
}

void VulkanRenderer::end_frame()
{
  _record_commands();
  _submit_commands();
  _present_swapchain_image();

  m_frame_index = (m_frame_index + 1) % m_frames.size();
}

auto VulkanRenderer::load_texture(const std::filesystem::path& image_path)
    -> std::expected<TextureID, std::error_code>
{
  auto texture = load_vulkan_texture(m_device.handle,
                                     m_graphics_queue,
                                     m_graphics_command_pool.handle,
                                     m_allocator.handle,
                                     m_sampler.handle,
                                     image_path);

  if (!texture.has_value()) {
    return std::unexpected {std::make_error_code(std::errc::io_error)};
  }

  const auto id = m_next_texture_id;
  ++m_next_texture_id.value;

  m_textures.insert_or_assign(id, std::move(*texture));

  return id;
}

void VulkanRenderer::unload_texture(const TextureID id)
{
  m_textures.erase(id);
}

auto VulkanRenderer::find_texture(const TextureID id) const -> const ITexture*
{
  const auto iter = m_textures.find(id);

  if (iter != m_textures.end()) {
    return &iter->second;
  }

  return nullptr;
}

void VulkanRenderer::try_reload_fonts()
{
  ImGui_ImplVulkan_CreateFontsTexture();
}

auto VulkanRenderer::can_reload_fonts() const -> bool
{
  return true;
}

auto VulkanRenderer::get_window() -> IWindow*
{
  return nullptr;
}

auto VulkanRenderer::get_window() const -> const IWindow*
{
  return nullptr;
}

auto VulkanRenderer::get_imgui_context() -> ImGuiContext*
{
  return m_imgui_context.get();
}

auto VulkanRenderer::imgui_malloc(const std::size_t bytes) -> void*
{
  return runtime_malloc(bytes);
}

void VulkanRenderer::imgui_free(void* memory)
{
  runtime_free(memory);
}

void VulkanRenderer::process_event(const SDL_Event& event)
{
  ImGui_ImplSDL2_ProcessEvent(&event);
}

void VulkanRenderer::_record_commands() const
{
  const auto& frame = m_frames.at(m_frame_index);

  if (_reset_and_begin_command_buffer(frame) != VK_SUCCESS) {
    return;
  }

  _prepare_swapchain_image_for_rendering(frame);
  _begin_dynamic_rendering(frame);
  _render_content(frame);
  _end_dynamic_rendering(frame);
  _prepare_swapchain_image_for_presentation(frame);
  _end_command_buffer(frame);
}

auto VulkanRenderer::_reset_and_begin_command_buffer(const VulkanFrame& frame) -> VkResult
{
  auto result = vkResetCommandBuffer(frame.command_buffer.handle, 0);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not reset command buffer: {}", to_string(result));
    return result;
  }

  constexpr VkCommandBufferBeginInfo command_buffer_begin_info {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = nullptr,
    .flags = 0,
    .pInheritanceInfo = nullptr,
  };

  result = vkBeginCommandBuffer(frame.command_buffer.handle, &command_buffer_begin_info);
  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not begin command buffer: {}", to_string(result));
  }

  return result;
}

void VulkanRenderer::_prepare_swapchain_image_for_rendering(const VulkanFrame& frame) const
{
  const VkImageMemoryBarrier color_attachment_barrier {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = nullptr,
    .srcAccessMask = 0,
    .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = m_swapchain.images.at(m_swapchain.image_index),
    .subresourceRange =
        VkImageSubresourceRange {
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,
        },
  };

  vkCmdPipelineBarrier(frame.command_buffer.handle,
                       VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                       0,
                       0,
                       nullptr,
                       0,
                       nullptr,
                       1,
                       &color_attachment_barrier);

  const VkImageMemoryBarrier depth_attachment_barrier {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = nullptr,
    .srcAccessMask = 0,
    .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = m_swapchain.depth_buffer.handle,
    .subresourceRange =
        VkImageSubresourceRange {
          .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,
        },
  };

  vkCmdPipelineBarrier(
      frame.command_buffer.handle,
      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
      0,
      0,
      nullptr,
      0,
      nullptr,
      1,
      &depth_attachment_barrier);
}

void VulkanRenderer::_begin_dynamic_rendering(const VulkanFrame& frame) const
{
  VkClearValue clear_values[2] = {};
  clear_values[0].color = VkClearColorValue {.float32 = {0, 0, 0, 1}};
  clear_values[1].depthStencil = VkClearDepthStencilValue {.depth = 1, .stencil = 0};

  const VkRenderingAttachmentInfoKHR color_attachment_info {
    .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
    .pNext = nullptr,
    .imageView = m_swapchain.image_views.at(m_swapchain.image_index).handle,
    .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    .resolveMode = VK_RESOLVE_MODE_NONE,
    .resolveImageView = VK_NULL_HANDLE,
    .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .clearValue = clear_values[0],
  };

  const VkRenderingAttachmentInfoKHR depth_attachment_info {
    .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
    .pNext = nullptr,
    .imageView = m_swapchain.depth_buffer_view.handle,
    .imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
    .resolveMode = VK_RESOLVE_MODE_NONE,
    .resolveImageView = VK_NULL_HANDLE,
    .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .clearValue = clear_values[1],
  };

  const auto& image_extent = m_swapchain.params.image_extent;

  const VkRenderingInfoKHR rendering_info {
    .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
    .pNext = nullptr,
    .flags = 0,
    .renderArea =
        VkRect2D {VkOffset2D {0, 0}, VkExtent2D {image_extent.width, image_extent.height}},
    .layerCount = 1,
    .viewMask = 0,
    .colorAttachmentCount = 1,
    .pColorAttachments = &color_attachment_info,
    .pDepthAttachment = &depth_attachment_info,
    .pStencilAttachment = nullptr,
  };

  m_vkCmdBeginRendering(frame.command_buffer.handle, &rendering_info);
}

void VulkanRenderer::_render_content(const VulkanFrame& frame) const
{
  ImGui::Render();
  ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                  frame.command_buffer.handle,
                                  m_imgui_context.get_pipeline());
}

void VulkanRenderer::_end_dynamic_rendering(const VulkanFrame& frame) const
{
  m_vkCmdEndRendering(frame.command_buffer.handle);
}

void VulkanRenderer::_prepare_swapchain_image_for_presentation(const VulkanFrame& frame) const
{
  const VkImageMemoryBarrier color_attachment_presentation_barrier {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = nullptr,
    .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    .dstAccessMask = VK_ACCESS_NONE,
    .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = m_swapchain.images.at(m_swapchain.image_index),
    .subresourceRange =
        VkImageSubresourceRange {
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,
        },
  };

  vkCmdPipelineBarrier(frame.command_buffer.handle,
                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                       VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                       0,
                       0,
                       nullptr,
                       0,
                       nullptr,
                       1,
                       &color_attachment_presentation_barrier);
}

void VulkanRenderer::_end_command_buffer(const VulkanFrame& frame)
{
  if (const auto result = vkEndCommandBuffer(frame.command_buffer.handle);
      result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not end Vulkan command buffer: {}", to_string(result));
  }
}

void VulkanRenderer::_submit_commands()
{
  auto& frame = m_frames.at(m_frame_index);

  // Wait for image_available_semaphore before executing the commands
  const VkSemaphore wait_semaphores[] = {
    frame.image_available_semaphore.handle,
  };

  // Signal render_finished_semaphore after the commands have finished.
  const VkSemaphore signal_semaphores[] = {
    frame.render_finished_semaphore.handle,
  };

  constexpr VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  const VkSubmitInfo submit_info {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext = nullptr,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = wait_semaphores,
    .pWaitDstStageMask = &wait_stage,
    .commandBufferCount = 1,
    .pCommandBuffers = &frame.command_buffer.handle,
    .signalSemaphoreCount = 1,
    .pSignalSemaphores = signal_semaphores,
  };

  const auto submit_result =
      vkQueueSubmit(m_graphics_queue, 1, &submit_info, frame.in_flight_fence.handle);

  if (submit_result != VK_SUCCESS) {
    log(LogLevel::kError,
        "Could not submit commands to Vulkan graphics queue: {}",
        to_string(submit_result));
  }
}

void VulkanRenderer::_present_swapchain_image()
{
  const auto& frame = m_frames.at(m_frame_index);

  const VkSemaphore wait_semaphores[] = {
    frame.render_finished_semaphore.handle,
  };

  const VkSwapchainKHR swapchains[] = {
    m_swapchain.handle,
  };

  const std::uint32_t image_indices[] = {
    m_swapchain.image_index,
  };

  const VkPresentInfoKHR present_info {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext = nullptr,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = wait_semaphores,
    .swapchainCount = 1,
    .pSwapchains = swapchains,
    .pImageIndices = image_indices,
    .pResults = nullptr,
  };

  auto result = vkQueuePresentKHR(m_present_queue, &present_info);

  if (result == VK_SUCCESS) {
    return;
  }

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    log(LogLevel::kDebug, "Vulkan swapchain is outdated");
    result = _recreate_swapchain();
  }
  else if (result == VK_SUBOPTIMAL_KHR) {
    log(LogLevel::kDebug, "Vulkan swapchain is suboptimal");
    result = _recreate_swapchain();
  }

  if (result != VK_SUCCESS) {
    log(LogLevel::kWarn,
        "Could not present image from Vulkan swapchain: {}",
        to_string(result));
  }
}

auto VulkanRenderer::_recreate_swapchain() -> VkResult
{
  log(LogLevel::kDebug, "Recreating Vulkan swapchain");

  // Avoid touching resources that may still be in use
  if (const auto wait_result = vkDeviceWaitIdle(m_device.handle); wait_result != VK_SUCCESS) {
    return wait_result;
  }

  VkSurfaceCapabilitiesKHR surface_capabilities {};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device,
                                            m_surface.handle,
                                            &surface_capabilities);

  auto new_params = m_swapchain.params;
  new_params.image_extent = surface_capabilities.currentExtent;

  log(LogLevel::kDebug,
      "New swapchain image extent: {{{}, {}}}",
      new_params.image_extent.width,
      new_params.image_extent.height);

  auto new_swapchain = create_vulkan_swapchain(m_surface.handle,
                                               m_device.handle,
                                               m_allocator.handle,
                                               new_params,
                                               m_swapchain.handle);
  if (!new_swapchain.has_value()) {
    log(LogLevel::kError,
        "Could not recreate Vulkan swapchain: {}",
        to_string(new_swapchain.error()));
    return new_swapchain.error();
  }

  m_swapchain = std::move(*new_swapchain);
  return VK_SUCCESS;
}

}  // namespace tactile

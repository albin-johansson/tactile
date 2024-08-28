// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>   // uint32_t
#include <expected>  // expected

#include <volk.h>

#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

class TACTILE_VULKAN_API VulkanDevice final
{
 public:
  TACTILE_DELETE_COPY(VulkanDevice);
  TACTILE_DECLARE_MOVE(VulkanDevice);

  VulkanDevice() = default;

  VulkanDevice(VkDevice device,
               std::uint32_t graphics_queue_index,
               std::uint32_t present_queue_index);

  ~VulkanDevice() noexcept;

  [[nodiscard]]
  static auto create(VkPhysicalDevice physical_device,
                     VkSurfaceKHR surface) -> std::expected<VulkanDevice, VkResult>;

  [[nodiscard]]
  auto get() -> VkDevice;

  [[nodiscard]]
  auto graphics_queue_index() const -> std::uint32_t;

  [[nodiscard]]
  auto presentation_queue_index() const -> std::uint32_t;

 private:
  VkDevice m_device {VK_NULL_HANDLE};
  std::uint32_t m_graphics_queue_index {};
  std::uint32_t m_present_queue_index {};

  void _destroy() noexcept;
};

}  // namespace tactile

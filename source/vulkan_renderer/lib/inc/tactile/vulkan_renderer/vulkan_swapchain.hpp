// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>   // uint32_t
#include <expected>  // expected
#include <vector>    // vector

#include <vulkan/vulkan.h>

#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"
#include "tactile/vulkan_renderer/vulkan_image.hpp"
#include "tactile/vulkan_renderer/vulkan_image_view.hpp"

namespace tactile {

struct VulkanSwapchainParams final
{
  VkExtent2D image_extent;
  VkFormat image_format;
  VkFormat depth_format;
  VkColorSpaceKHR image_color_space;
  VkSharingMode image_sharing_mode;
  VkPresentModeKHR present_mode;
  VkSurfaceTransformFlagBitsKHR pre_transform;
  std::uint32_t min_image_count;
  std::vector<std::uint32_t> queue_family_indices;
};

class TACTILE_VULKAN_API VulkanSwapchain final
{
 public:
  TACTILE_DELETE_COPY(VulkanSwapchain);
  TACTILE_DECLARE_MOVE(VulkanSwapchain);

  VulkanSwapchain(VkSurfaceKHR surface,
                  VkDevice device,
                  VmaAllocator allocator,
                  VkSwapchainKHR swapchain,
                  VulkanSwapchainParams params,
                  std::vector<VkImage> images,
                  std::vector<VulkanImageView> image_views,
                  VulkanImage depth_image,
                  VulkanImageView depth_image_view);

  ~VulkanSwapchain() noexcept;

  [[nodiscard]]
  static auto create(VkSurfaceKHR surface,
                     VkDevice device,
                     VmaAllocator allocator,
                     VulkanSwapchainParams params,
                     VkSwapchainKHR old_swapchain = VK_NULL_HANDLE)
      -> std::expected<VulkanSwapchain, VkResult>;

  [[nodiscard]]
  static auto create(VkSurfaceKHR surface,
                     VkPhysicalDevice physical_device,
                     VkDevice device,
                     VmaAllocator allocator,
                     VkExtent2D image_extent) -> std::expected<VulkanSwapchain, VkResult>;

  [[nodiscard]]
  auto params() const -> const VulkanSwapchainParams&;

  [[nodiscard]]
  auto image_count() const -> std::size_t;

 private:
  VkSurfaceKHR m_surface;
  VkDevice m_device;
  VmaAllocator m_allocator;
  VkSwapchainKHR m_swapchain;
  VulkanSwapchainParams m_params;
  std::vector<VkImage> m_images;
  std::vector<VulkanImageView> m_image_views;
  VulkanImage m_depth_image;
  VulkanImageView m_depth_image_view;
  std::uint32_t m_image_index {0};

  void _destroy() noexcept;
};

}  // namespace tactile

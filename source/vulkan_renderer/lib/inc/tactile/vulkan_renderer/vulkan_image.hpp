// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>   // uint32_t
#include <expected>  // expected

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

struct VulkanImageParams final
{
  VkImageType type;
  VkFormat format;
  VkImageLayout layout;
  VkExtent2D extent;
  VkImageUsageFlags usage;
  std::uint32_t mip_levels;
};

class TACTILE_VULKAN_API VulkanImage final
{
 public:
  TACTILE_DELETE_COPY(VulkanImage);
  TACTILE_DECLARE_MOVE(VulkanImage);

  VulkanImage() = default;

  ~VulkanImage() noexcept;

  VmaAllocator allocator {VK_NULL_HANDLE};
  VkImage handle {VK_NULL_HANDLE};
  VmaAllocation allocation {VK_NULL_HANDLE};
  VulkanImageParams params {};

 private:
  void _destroy() noexcept;
};

[[nodiscard]]
TACTILE_VULKAN_API auto create_vulkan_image(VmaAllocator allocator,
                                            const VulkanImageParams& params)
    -> std::expected<VulkanImage, VkResult>;

}  // namespace tactile

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

  VulkanImage(VmaAllocator allocator,
              VkImage image,
              VmaAllocation allocation,
              const VulkanImageParams& params);

  ~VulkanImage() noexcept;

  [[nodiscard]]
  static auto create(VmaAllocator allocator,
                     const VulkanImageParams& params) -> std::expected<VulkanImage, VkResult>;

  [[nodiscard]]
  auto get() noexcept -> VkImage
  {
    return m_image;
  }

  [[nodiscard]]
  auto params() const noexcept -> const VulkanImageParams&
  {
    return m_params;
  }

 private:
  VmaAllocator m_allocator {VK_NULL_HANDLE};
  VkImage m_image {VK_NULL_HANDLE};
  VmaAllocation m_allocation {VK_NULL_HANDLE};
  VulkanImageParams m_params {};

  void _destroy() noexcept;
};

}  // namespace tactile

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_swapchain.hpp"

#include <algorithm>  // clamp
#include <utility>    // move, exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_physical_device.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {
namespace {

[[nodiscard]]
auto _pick_image_format(VkPhysicalDevice physical_device,
                        VkSurfaceKHR surface) -> std::expected<VkSurfaceFormatKHR, VkResult>
{
  std::vector<VkSurfaceFormatKHR> surface_formats {};

  std::uint32_t surface_format_count {};
  auto result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
                                                     surface,
                                                     &surface_format_count,
                                                     nullptr);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not get Vulkan surface format count: {}", to_string(result));
    return std::unexpected {result};
  }

  surface_formats.resize(surface_format_count);
  result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
                                                surface,
                                                &surface_format_count,
                                                surface_formats.data());

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not get Vulkan surface formats: {}", to_string(result));
    return std::unexpected {result};
  }

  for (const auto& format : surface_formats) {
    if (format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR &&
        (format.format == VK_FORMAT_B8G8R8A8_SRGB ||
         format.format == VK_FORMAT_R8G8B8A8_SRGB)) {
      return format;
    }
  }

  return surface_formats.at(0);
}

[[nodiscard]]
auto _pick_present_mode(VkPhysicalDevice physical_device,
                        VkSurfaceKHR surface) -> std::expected<VkPresentModeKHR, VkResult>
{
  std::vector<VkPresentModeKHR> present_modes {};

  std::uint32_t present_mode_count {};
  auto result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                                          surface,
                                                          &present_mode_count,
                                                          nullptr);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not get Vulkan present mode count: {}", to_string(result));
    return std::unexpected {result};
  }

  present_modes.resize(present_mode_count);
  result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                                     surface,
                                                     &present_mode_count,
                                                     present_modes.data());

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not get Vulkan present modes: {}", to_string(result));
    return std::unexpected {result};
  }

  for (const auto present_mode : present_modes) {
    if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return present_mode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

[[nodiscard]]
auto _get_swapchain_images(VkDevice device, VkSwapchainKHR swapchain)
    -> std::expected<std::vector<VkImage>, VkResult>
{
  std::vector<VkImage> images {};

  std::uint32_t image_count {};
  auto result = vkGetSwapchainImagesKHR(device, swapchain, &image_count, nullptr);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not get Vulkan swapchain image count: {}", to_string(result));
    return std::unexpected {result};
  }

  images.resize(image_count);
  result = vkGetSwapchainImagesKHR(device, swapchain, &image_count, images.data());

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not get Vulkan swapchain images: {}", to_string(result));
    return std::unexpected {result};
  }

  return images;
}

void _prepare_depth_buffer(VkDevice device,
                           VmaAllocator allocator,
                           const VkFormat format,
                           const VkExtent2D extent,
                           VulkanImage& depth_image,
                           VulkanImageView& depth_image_view)
{
  const VulkanImageParams depth_image_params {
    .type = VK_IMAGE_TYPE_2D,
    .format = format,
    .layout = VK_IMAGE_LAYOUT_UNDEFINED,
    .extent = extent,
    .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
             VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    .mip_levels = 1,
  };

  depth_image = VulkanImage::create(allocator, depth_image_params).value();
  depth_image_view = VulkanImageView::create(device,
                                             depth_image.get(),
                                             depth_image_params.format,
                                             VK_IMAGE_VIEW_TYPE_2D,
                                             VK_IMAGE_ASPECT_DEPTH_BIT,
                                             1)
                         .value();
}

}  // namespace

VulkanSwapchain::VulkanSwapchain(VkSurfaceKHR surface,
                                 VkDevice device,
                                 VmaAllocator allocator,
                                 VkSwapchainKHR swapchain,
                                 VulkanSwapchainParams params,
                                 std::vector<VkImage> images,
                                 std::vector<VulkanImageView> image_views,
                                 VulkanImage depth_image,
                                 VulkanImageView depth_image_view)
    : m_surface {surface},
      m_device {device},
      m_allocator {allocator},
      m_swapchain {swapchain},
      m_params {std::move(params)},
      m_images {std::move(images)},
      m_image_views {std::move(image_views)},
      m_depth_image {std::move(depth_image)},
      m_depth_image_view {std::move(depth_image_view)}
{}

VulkanSwapchain::VulkanSwapchain(VulkanSwapchain&& other) noexcept
    : m_surface {std::exchange(other.m_surface, VK_NULL_HANDLE)},
      m_device {std::exchange(other.m_device, VK_NULL_HANDLE)},
      m_allocator {std::exchange(other.m_allocator, VK_NULL_HANDLE)},
      m_swapchain {std::exchange(other.m_swapchain, VK_NULL_HANDLE)},
      m_params {std::exchange(other.m_params, VulkanSwapchainParams {})},
      m_images {std::exchange(other.m_images, {})},
      m_image_views {std::exchange(other.m_image_views, {})},
      m_depth_image {std::exchange(other.m_depth_image, VulkanImage {})},
      m_depth_image_view {std::exchange(other.m_depth_image_view, VulkanImageView {})},
      m_image_index {std::exchange(other.m_image_index, 0)}
{}

VulkanSwapchain::~VulkanSwapchain() noexcept
{
  _destroy();
}

auto VulkanSwapchain::operator=(VulkanSwapchain&& other) noexcept -> VulkanSwapchain&
{
  if (this != &other) {
    _destroy();

    m_surface = std::exchange(other.m_surface, VK_NULL_HANDLE);
    m_device = std::exchange(other.m_device, VK_NULL_HANDLE);
    m_allocator = std::exchange(other.m_allocator, VK_NULL_HANDLE);
    m_swapchain = std::exchange(other.m_swapchain, VK_NULL_HANDLE);
    m_params = std::exchange(other.m_params, VulkanSwapchainParams {});
    m_images = std::exchange(other.m_images, {});
    m_image_views = std::exchange(other.m_image_views, {});
    m_depth_image = std::exchange(other.m_depth_image, VulkanImage {});
    m_depth_image_view = std::exchange(other.m_depth_image_view, VulkanImageView {});
    m_image_index = std::exchange(other.m_image_index, 0);
  }

  return *this;
}

void VulkanSwapchain::_destroy() noexcept
{
  if (m_swapchain != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
    m_swapchain = VK_NULL_HANDLE;
  }
}

auto VulkanSwapchain::create(VkSurfaceKHR surface,
                             VkDevice device,
                             VmaAllocator allocator,
                             VulkanSwapchainParams params,
                             VkSwapchainKHR old_swapchain)
    -> std::expected<VulkanSwapchain, VkResult>
{
  const VkSwapchainCreateInfoKHR swapchain_info {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = nullptr,
    .flags = 0,
    .surface = surface,
    .minImageCount = params.min_image_count,
    .imageFormat = params.image_format,
    .imageColorSpace = params.image_color_space,
    .imageExtent = params.image_extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode = params.image_sharing_mode,
    .queueFamilyIndexCount = static_cast<std::uint32_t>(params.queue_family_indices.size()),
    .pQueueFamilyIndices = params.queue_family_indices.data(),
    .preTransform = params.pre_transform,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = params.present_mode,
    .clipped = VK_TRUE,
    .oldSwapchain = old_swapchain,
  };

  VkSwapchainKHR swapchain {};
  const auto result = vkCreateSwapchainKHR(device, &swapchain_info, nullptr, &swapchain);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan swapchain: {}", to_string(result));
    return std::unexpected {result};
  }

  auto images = _get_swapchain_images(device, swapchain);
  if (!images.has_value()) {
    return std::unexpected {images.error()};
  }

  std::vector<VulkanImageView> image_views {};
  image_views.reserve(images->size());
  for (VkImage image : *images) {
    auto image_view = VulkanImageView::create(device,
                                              image,
                                              params.image_format,
                                              VK_IMAGE_VIEW_TYPE_2D,
                                              VK_IMAGE_ASPECT_COLOR_BIT,
                                              1)
                          .value();
    image_views.push_back(std::move(image_view));
  }

  VulkanImage depth_image {};
  VulkanImageView depth_image_view {};
  _prepare_depth_buffer(device,
                        allocator,
                        params.depth_format,
                        params.image_extent,
                        depth_image,
                        depth_image_view);

  return VulkanSwapchain {surface,
                          device,
                          allocator,
                          swapchain,
                          std::move(params),
                          std::move(*images),
                          std::move(image_views),
                          std::move(depth_image),
                          std::move(depth_image_view)};
}

auto VulkanSwapchain::create(VkSurfaceKHR surface,
                             VkPhysicalDevice physical_device,
                             VkDevice device,
                             VmaAllocator allocator,
                             VkExtent2D image_extent)
    -> std::expected<VulkanSwapchain, VkResult>
{
  const auto queue_family_indices = get_queue_family_indices(physical_device, surface);
  auto unique_queue_family_indices = to_unique(queue_family_indices);

  VkSurfaceCapabilitiesKHR capabilities {};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities);

  const auto image_width = std::clamp(image_extent.width,
                                      capabilities.minImageExtent.width,
                                      capabilities.maxImageExtent.width);

  const auto image_height = std::clamp(image_extent.height,
                                       capabilities.minImageExtent.height,
                                       capabilities.maxImageExtent.height);

  const auto min_image_count = std::clamp(capabilities.minImageCount + 1,
                                          capabilities.minImageCount,
                                          capabilities.maxImageCount);

  const auto image_sharing_mode = queue_family_indices.graphics != queue_family_indices.present
                                      ? VK_SHARING_MODE_CONCURRENT
                                      : VK_SHARING_MODE_EXCLUSIVE;

  const auto surface_format = _pick_image_format(physical_device, surface);
  if (!surface_format.has_value()) {
    return std::unexpected {surface_format.error()};
  }

  const auto present_mode = _pick_present_mode(physical_device, surface);
  if (!present_mode.has_value()) {
    return std::unexpected {present_mode.error()};
  }

  const VulkanSwapchainParams params {
    .image_extent = VkExtent2D {image_width, image_height},
    .image_format = surface_format->format,
    .depth_format = VK_FORMAT_D32_SFLOAT,
    .image_color_space = surface_format->colorSpace,
    .image_sharing_mode = image_sharing_mode,
    .present_mode = *present_mode,
    .pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    .min_image_count = min_image_count,
    .queue_family_indices = std::move(unique_queue_family_indices),
  };

  return create(surface, device, allocator, params);
}

auto VulkanSwapchain::params() const -> const VulkanSwapchainParams&
{
  return m_params;
}

auto VulkanSwapchain::image_count() const -> std::size_t
{
  return m_images.size();
}

}  // namespace tactile

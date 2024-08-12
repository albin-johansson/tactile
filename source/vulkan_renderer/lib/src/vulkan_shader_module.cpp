// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_shader_module.hpp"

#include <utility>  // exchange

#include "tactile/base/io/file_io.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanShaderModule::VulkanShaderModule(VkDevice device, VkShaderModule shader_module) noexcept
  : mDevice {device},
    mShaderModule {shader_module}
{}

VulkanShaderModule::VulkanShaderModule(VulkanShaderModule&& other) noexcept
  : mDevice {std::exchange(other.mDevice, VK_NULL_HANDLE)},
    mShaderModule {std::exchange(other.mShaderModule, VK_NULL_HANDLE)}
{}

VulkanShaderModule::~VulkanShaderModule() noexcept
{
  _destroy();
}

void VulkanShaderModule::_destroy() noexcept
{
  if (mShaderModule != VK_NULL_HANDLE) {
    vkDestroyShaderModule(mDevice, mShaderModule, nullptr);
    mShaderModule = VK_NULL_HANDLE;
  }
}

auto VulkanShaderModule::operator=(VulkanShaderModule&& other) noexcept -> VulkanShaderModule&
{
  if (this != &other) {
    _destroy();

    mDevice = std::exchange(other.mDevice, VK_NULL_HANDLE);
    mShaderModule = std::exchange(other.mShaderModule, VK_NULL_HANDLE);
  }

  return *this;
}

auto VulkanShaderModule::load(VkDevice device, const Path& shader_path)
    -> Expected<VulkanShaderModule, VkResult>
{
  const auto shader_code = read_binary_file(shader_path);

  if (!shader_code.has_value()) {
    log(LogLevel::kError, "Could not load Vulkan shader file");
    return unexpected(VK_ERROR_UNKNOWN);
  }

  const VkShaderModuleCreateInfo create_info {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .codeSize = shader_code->size(),
    .pCode = reinterpret_cast<const uint32*>(shader_code->data()),
  };

  VkShaderModule shader_module {};
  const auto result = vkCreateShaderModule(device, &create_info, nullptr, &shader_module);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan shader module: {}", to_string(result));
    return unexpected(result);
  }

  return VulkanShaderModule {device, shader_module};
}

}  // namespace tactile

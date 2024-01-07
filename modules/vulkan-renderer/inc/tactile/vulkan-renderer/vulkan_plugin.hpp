// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/plugin/plugin.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/vulkan-renderer/api.hpp"

namespace tactile::vulkan {

class VulkanWindow;
class VulkanRenderer;

/**
 * Plugin that provides a Vulkan 1.2 renderer backend.
 */
class TACTILE_VULKAN_API VulkanPlugin final : public IPlugin {
 public:
  ~VulkanPlugin() noexcept override;

  void on_load() override;

  void on_unload() noexcept override;

 private:
  Unique<VulkanWindow> mWindow;
  Unique<VulkanRenderer> mRenderer;
};

extern "C"
{
  [[nodiscard]]
  TACTILE_VULKAN_API auto tactile_create_plugin() -> IPlugin*;

  TACTILE_VULKAN_API void tactile_destroy_plugin(IPlugin* plugin);
}

}  // namespace tactile::vulkan

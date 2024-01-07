// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan-renderer/vulkan_plugin.hpp"

#include <exception>  // exception
#include <utility>    // move

#include <SDL2/SDL.h>

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/vulkan-renderer/vulkan_renderer.hpp"
#include "tactile/vulkan-renderer/vulkan_window.hpp"

namespace tactile::vulkan {

VulkanPlugin::~VulkanPlugin() noexcept = default;

void VulkanPlugin::on_load()
{
  TACTILE_LOG_DEBUG("Loading Vulkan renderer plugin...");

  if (SDL_WasInit(SDL_INIT_EVERYTHING) != SDL_INIT_EVERYTHING) {
    TACTILE_LOG_ERROR("[Vulkan] SDL was not initialized");
    return;
  }

  try {
    VulkanWindow::create()
        .and_then([this](VulkanWindow&& window) {
          mWindow = make_unique<VulkanWindow>(std::move(window));
          return kOK;
        })
        .and_then([this] { return VulkanRenderer::create(mWindow.get()); })
        .and_then([this](VulkanRenderer&& renderer) {
          mRenderer = make_unique<VulkanRenderer>(std::move(renderer));
          return kOK;
        })
        .or_else([](const ErrorCode& error_code) {
          TACTILE_LOG_ERROR("[Vulkan] Could not load plugin: {}", error_code.message());
          return kOK;
        });
  }
  catch (const Exception& err) {
    TACTILE_LOG_ERROR("[Vulkan] Error: {}\n{}", err.what(), err.get_trace());
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("[Vulkan] Error: {}", ex.what());
  }
}

void VulkanPlugin::on_unload() noexcept
{
  TACTILE_LOG_DEBUG("Unloading Vulkan renderer plugin...");

  mRenderer.reset();
  mWindow.reset();
}

auto tactile_create_plugin() -> IPlugin*
{
  return new VulkanPlugin {};
}

void tactile_destroy_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile::vulkan

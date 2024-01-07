// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan-renderer/vulkan_renderer.hpp"

#include <utility>  // exchange

#include <SDL2/SDL_vulkan.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/vulkan-renderer/vulkan_error.hpp"
#include "tactile/vulkan-renderer/vulkan_window.hpp"

namespace tactile::vulkan {

auto VulkanRenderer::create(VulkanWindow* window) -> Result<VulkanRenderer>
{
  auto* imgui_context = ImGui::CreateContext();
  if (!imgui_context) {
    TACTILE_LOG_ERROR("[Vulkan] Could not create Dear ImGui context");
    return unexpected(make_generic_error(GenericError::kInitFailure));
  }

  return VulkanRenderer {window, imgui_context};
}

VulkanRenderer::VulkanRenderer(VulkanWindow* window, ImGuiContext* imgui_context)
  : mWindow {require_not_null(window, "null window")},
    mImGuiContext {require_not_null(imgui_context, "null ImGui context")},
    mPrimed {true}
{}

VulkanRenderer::~VulkanRenderer() noexcept
{
  _dispose();
}

void VulkanRenderer::_dispose() noexcept
{
  if (mPrimed) {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    ImGui::DestroyContext(mImGuiContext);
    mImGuiContext = nullptr;

    mPrimed = false;
  }
}

VulkanRenderer::VulkanRenderer(VulkanRenderer&& other) noexcept
  : mWindow {std::exchange(other.mWindow, nullptr)},
    mImGuiContext {std::exchange(other.mImGuiContext, nullptr)},
    mPrimed {std::exchange(other.mPrimed, false)}
{}

auto VulkanRenderer::operator=(VulkanRenderer&& other) noexcept -> VulkanRenderer&
{
  if (this != &other) {
    _dispose();

    mWindow = std::exchange(other.mWindow, nullptr);
    mImGuiContext = std::exchange(other.mImGuiContext, nullptr);
    mPrimed = std::exchange(other.mPrimed, false);
  }

  return *this;
}

auto VulkanRenderer::begin_frame() -> Result<void>
{
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

void VulkanRenderer::end_frame()
{}

void VulkanRenderer::reload_fonts_texture()
{}

auto VulkanRenderer::can_reload_fonts_texture() const -> bool
{
  return false;
}

auto VulkanRenderer::load_texture(const FilePath&) -> ITexture*
{
  return nullptr;
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
  return nullptr;
}

}  // namespace tactile::vulkan

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_imgui_context.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>

#include "tactile/base/render/window.hpp"
#include "tactile/base/util/scope_exit.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

auto VulkanImGuiContext::init(IWindow& window, ImGui_ImplVulkan_InitInfo& vulkan_info)
    -> std::optional<VulkanImGuiContext>
{
  VulkanImGuiContext context {};

  context.m_ctx = ImGui::CreateContext();
  if (!context.m_ctx) {
    log(LogLevel::kError, "Could not create ImGui context");
    return std::nullopt;
  }

  context.m_context_deleter = ScopeExit {[ctx = context.m_ctx] {
    ImGui::DestroyContext(ctx);
  }};

  if (!ImGui_ImplSDL2_InitForVulkan(window.get_handle())) {
    log(LogLevel::kError, "Could not initialize SDL2 ImGui backend implementation");
    return std::nullopt;
  }

  context.m_backend_impl_deleter = ScopeExit {[] {
    ImGui_ImplSDL2_Shutdown();
  }};

  if (!ImGui_ImplVulkan_Init(&vulkan_info)) {
    log(LogLevel::kError, "Could not initialize Vulkan ImGui renderer implementation");
    return std::nullopt;
  }

  context.m_renderer_impl_deleter = ScopeExit {[] {
    ImGui_ImplVulkan_Shutdown();
  }};

  return context;
}

auto VulkanImGuiContext::get() const -> ImGuiContext*
{
  return m_ctx;
}

}  // namespace tactile

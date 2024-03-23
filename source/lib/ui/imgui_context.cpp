// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "imgui_context.hpp"

#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <spdlog/spdlog.h>

#include "model/settings.hpp"
#include "tactile/core/platform/filesystem.hpp"
#include "ui/style/themes.hpp"

namespace tactile {
namespace {

constexpr auto kFontRobotoPath = "assets/fonts/roboto/Roboto-Regular.ttf";
constexpr auto kFontFaPath = "assets/fonts/fa/fa-solid-900.ttf";
constexpr ImWchar kFontIconRange[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

}  // namespace

ImGuiContext::ImGuiContext()
{
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= static_cast<ImGuiConfigFlags>(ImGuiConfigFlags_DockingEnable);
  io.WantCaptureKeyboard = true;

  static const auto ini = get_imgui_ini_file_path().string();
  io.IniFilename = ini.c_str();

  ImGui::StyleColorsDark();

  const auto& settings = get_settings();
  auto& style = ImGui::GetStyle();

  ui::apply_style(style);
  apply_theme(style, settings.get_theme(), settings.get_theme_saturation());

  style.WindowBorderSize = settings.test_flag(SETTINGS_WINDOW_BORDER_BIT) ? 1.0f : 0.0f;

  spdlog::debug("Initialized renderer backend... {}", mInitializedBackend ? "yes" : "no");
}

void ImGuiContext::reload_fonts()
{
  spdlog::debug("Reloading fonts...");

  auto& io = ImGui::GetIO();
  const auto scale = io.DisplayFramebufferScale;

  io.Fonts->Clear();

  const auto& settings = get_settings();
  const auto size = settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT)
                        ? 13.0f
                        : static_cast<float>(settings.get_font_size());

  if (settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT)) {
    ImFontConfig config {};
    config.SizePixels = size * scale.x;
    io.Fonts->AddFontDefault(&config);
  }
  else {
    io.Fonts->AddFontFromFileTTF(kFontRobotoPath, size * scale.x);
  }

  // The global scale is 1 on most platforms, and 0.5 on macOS
  io.FontGlobalScale = 1.0f / scale.x;

  ImFontConfig config {};
  config.MergeMode = true;
  config.GlyphMinAdvanceX = size * scale.x;
  config.GlyphMaxAdvanceX = config.GlyphMinAdvanceX;
  config.GlyphOffset = {0, 2};
  io.Fonts->AddFontFromFileTTF(kFontFaPath, size * scale.x, &config, kFontIconRange);

  io.Fonts->Build();

  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_CreateFontsTexture();

  ImGui::GetStyle().ScaleAllSizes(1.0f);
}

}  // namespace tactile

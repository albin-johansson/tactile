#include "imgui_context.hpp"

#include <IconsFontAwesome5.h>

#include <array>  // array

#include "gui/style.hpp"
#include "gui/themes.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

namespace tactile {
namespace {

inline constexpr auto glsl_version = "#version 130";

void LoadFonts()
{
  auto& io = ImGui::GetIO();

  ImFontConfig defaultConfig{};
  defaultConfig.SizePixels = 13.0f;
  io.Fonts->AddFontDefault(&defaultConfig);

  // Merge in icons from Font Awesome
  static const std::array<ImWchar, 3> range = {ICON_MIN_FA, ICON_MAX_FA, 0};
  ImFontConfig iconConfig{};
  iconConfig.MergeMode = true;
  iconConfig.PixelSnapH = true;
  iconConfig.GlyphOffset = {0, 2};

  io.Fonts->AddFontFromFileTTF("resources/fonts/fa/fa-solid-900.otf",
                               15,
                               &iconConfig,
                               range.data());
}

}  // namespace

ImGuiContext::ImGuiContext(cen::window& window, cen::gl_context& context)
{
  CENTURION_LOG_DEBUG("Creating ImGui context...");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // NOLINT
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // NOLINT
  io.WantCaptureKeyboard = true;

  LoadFonts();

  ImGui::StyleColorsDark();
  auto& style = ImGui::GetStyle();
  ApplyStyle(style);
  ApplyNightTheme(style);
  style.ScaleAllSizes(1);

  ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
  ImGui_ImplOpenGL3_Init(glsl_version);
}

ImGuiContext::~ImGuiContext()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace tactile

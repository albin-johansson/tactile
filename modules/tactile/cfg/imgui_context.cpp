#include "imgui_context.hpp"

#include <array>  // array

#include <IconsFontAwesome5.h>
#include <imgui.h>

#include "build.hpp"
#include "editor/gui/style.hpp"
#include "editor/gui/themes.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "io/preferences.hpp"
#include "logging.hpp"

namespace Tactile {
namespace {

void LoadIconFont(const float size)
{
  static constexpr std::array<ImWchar, 3> range = {ICON_MIN_FA, ICON_MAX_FA, 0};
  ImFontConfig config{};
  config.MergeMode = true;
  config.PixelSnapH = true;
  config.GlyphOffset = {0, 1};

  auto& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("resources/fonts/fa/fa-solid-900.otf",
                               size,
                               &config,
                               range.data());
}

void LoadFonts()
{
  auto& io = ImGui::GetIO();

  if constexpr (IsPlatformOSX()) {
    constexpr auto fontSize = 11.0f;

    const auto dpi = cen::GetDisplayDPI().value();
    const auto scaling = dpi.diagonal / 96.0f;

    io.FontGlobalScale = 0.5f;
    io.Fonts->AddFontFromFileTTF("resources/fonts/roboto/Roboto-Regular.ttf",
                                 scaling * fontSize);
    LoadIconFont(scaling * fontSize);
  }
  else {
    io.Fonts->AddFontDefault();
    LoadIconFont(13.0f);
  }
}

}  // namespace

ImGuiContext::ImGuiContext(cen::Window& window, cen::GLContext& context)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // NOLINT
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // NOLINT
  io.WantCaptureKeyboard = true;

  LoadFonts();

  ImGui::StyleColorsDark();

  auto& style = ImGui::GetStyle();
  ApplyStyle(style);
  ApplyTheme(style, Prefs::GetTheme());

  style.WindowBorderSize = Prefs::GetWindowBorder() ? 1.0f : 0.0f;

  ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
  if constexpr (IsPlatformOSX()) {
    mInitializedBackend = ImGui_ImplOpenGL3_Init("#version 150");
  }
  else {
    mInitializedBackend = ImGui_ImplOpenGL3_Init("#version 130");
  }

  LogDebug("Initialized renderer backend... {}", mInitializedBackend ? "yes" : "no");
}

ImGuiContext::~ImGuiContext()
{
  if (mInitializedBackend) {
    ImGui_ImplOpenGL3_Shutdown();
  }
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace Tactile
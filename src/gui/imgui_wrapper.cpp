#include "imgui_wrapper.hpp"

#include <IconsFontAwesome5.h>

#include <array>  // array

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "themes.hpp"

namespace tactile {
namespace {

inline constexpr auto glsl_version = "#version 130";

}  // namespace

ImGuiWrapper::ImGuiWrapper(cen::window& window, cen::gl_context& context)
{
  CENTURION_LOG_DEBUG("Initializing ImGui...");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // NOLINT
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // NOLINT
  io.WantCaptureKeyboard = true;

  io.Fonts->AddFontDefault();

  // Merge in icons from Font Awesome
  static const std::array<ImWchar, 3> range = {ICON_MIN_FA, ICON_MAX_FA, 0};
  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;

  io.Fonts->AddFontFromFileTTF("resources/fonts/fa/fa-solid-900.otf",
                               13.0f,
                               &icons_config,
                               range.data());

  ImGui::StyleColorsDark();

  auto& style = ImGui::GetStyle();
  style.WindowPadding = ImVec2{15, 15};
  style.FramePadding = ImVec2{5, 5};

  style.ItemSpacing = ImVec2{12, 8};
  style.ItemInnerSpacing = ImVec2{8, 6};
  style.IndentSpacing = 25;

  style.GrabMinSize = 6;
  style.ScrollbarSize = 10;

  style.WindowRounding = 5;
  style.FrameRounding = 4;
  style.PopupRounding = 4;
  style.ScrollbarRounding = 4;
  style.GrabRounding = 4;
  style.TabRounding = 4;

  CENTURION_LOG_DEBUG("Applying the \"Night\" theme...");
  ApplyNightTheme(style);

  style.ScaleAllSizes(1);

  ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
  ImGui_ImplOpenGL3_Init(glsl_version);
}

ImGuiWrapper::~ImGuiWrapper()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace tactile

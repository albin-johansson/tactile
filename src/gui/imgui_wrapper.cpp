#include "imgui_wrapper.hpp"

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
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // NOLINT
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // NOLINT
  io.WantCaptureKeyboard = true;

  io.Fonts->AddFontFromFileTTF(R"(resources\fonts\roboto\Roboto-Regular.ttf)",
                               15);

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

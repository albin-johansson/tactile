#include "imgui_wrapper.hpp"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

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

  ImGui::StyleColorsDark();

  auto& style = ImGui::GetStyle();
  style.FrameRounding = 4;
  style.WindowBorderSize = 0;

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

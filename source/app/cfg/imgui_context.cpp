#include "imgui_context.hpp"

#include <IconsFontAwesome5.h>
#include <imgui.h>

#include "editor/gui/themes.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "io/persistence/preferences.hpp"
#include "meta/build.hpp"
#include "misc/logging.hpp"

namespace tactile {
namespace {

void load_fonts()
{
  auto& io = ImGui::GetIO();
  io.Fonts->Clear();

  constexpr float font_size = 13.0f;
  constexpr float scale = on_osx ? 2.0f : 1.0f;  // io.DisplayFramebufferScale

  io.FontGlobalScale = 1.0f / scale;

  io.Fonts->AddFontFromFileTTF("resources/fonts/roboto/Roboto-Regular.ttf",
                               font_size * scale);

  ImFontConfig config{};
  config.MergeMode = true;
  config.PixelSnapH = true;
  config.GlyphOffset = {0, 2};
  config.GlyphMinAdvanceX = font_size * scale;
  config.GlyphMaxAdvanceX = config.GlyphMinAdvanceX;

  static constexpr ImWchar icon_range[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  io.Fonts->AddFontFromFileTTF("resources/fonts/fa/fa-solid-900.otf",
                               font_size * scale,
                               &config,
                               icon_range);

  io.Fonts->Build();
}

}  // namespace

im_gui_context::im_gui_context(cen::window& window, cen::gl_context& context)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // NOLINT
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // NOLINT
  io.WantCaptureKeyboard = true;

  ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
  if constexpr (on_osx) {
    mInitializedBackend = ImGui_ImplOpenGL3_Init("#version 150");
  }
  else {
    mInitializedBackend = ImGui_ImplOpenGL3_Init("#version 130");
  }

  load_fonts();

  ImGui::StyleColorsDark();

  const auto& prefs = get_preferences();
  auto& style = ImGui::GetStyle();

  apply_style(style);
  apply_theme(style, prefs.get_theme());

  style.WindowBorderSize = prefs.has_window_border() ? 1.0f : 0.0f;

  log_debug("Initialized renderer backend... {}", mInitializedBackend ? "yes" : "no");
}

im_gui_context::~im_gui_context()
{
  if (mInitializedBackend) {
    ImGui_ImplOpenGL3_Shutdown();
  }
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace tactile

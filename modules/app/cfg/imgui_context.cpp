#include "imgui_context.hpp"

#include <array>  // array

#include <IconsFontAwesome5.h>
#include <imgui.h>

#include "build.hpp"
#include "editor/gui/style.hpp"
#include "editor/gui/themes.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "io/persistence/preferences.hpp"
#include "logging.hpp"

namespace tactile {
namespace {

void load_icon_font(const float size)
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

void load_fonts()
{
  auto& io = ImGui::GetIO();

  if constexpr (on_osx) {
    constexpr auto fontSize = 11.0f;

    const auto dpi = cen::display_dpi().value();
    const auto scaling = dpi.diagonal / 96.0f;

    io.FontGlobalScale = 0.5f;
    io.Fonts->AddFontFromFileTTF("resources/fonts/roboto/Roboto-Regular.ttf",
                                 scaling * fontSize);
    load_icon_font(scaling * fontSize);
  }
  else {
    io.Fonts->AddFontDefault();
    load_icon_font(13.0f);
  }
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

  load_fonts();

  ImGui::StyleColorsDark();

  const auto& prefs = get_preferences();
  auto& style = ImGui::GetStyle();

  ApplyStyle(style);
  apply_theme(style, prefs.get_theme());

  style.WindowBorderSize = prefs.has_window_border() ? 1.0f : 0.0f;

  ImGui_ImplSDL2_InitForOpenGL(window.get(), context.get());
  if constexpr (on_osx) {
    mInitializedBackend = ImGui_ImplOpenGL3_Init("#version 150");
  }
  else {
    mInitializedBackend = ImGui_ImplOpenGL3_Init("#version 130");
  }

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

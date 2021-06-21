#include "application.hpp"

#include <utility>  // move

#include "gui/show_gui.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

namespace tactile {

Application::Application(cen::window&& window, cen::gl_context&& context)
    : mWindow{std::move(window)}
    , mContext{std::move(context)}
{}

auto Application::Run() -> int
{
  const auto& io = ImGui::GetIO();

  cen::event event;
  bool quit{};

  while (!quit)
  {
    while (event.poll())
    {
      ImGui_ImplSDL2_ProcessEvent(event.data());

      if (event.is<cen::quit_event>())
      {
        quit = true;
        break;
      }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(mWindow.get());
    ImGui::NewFrame();
    UpdateFrame();
    ImGui::EndFrame();

    ImGui::Render();

    glViewport(0,
               0,
               static_cast<int>(io.DisplaySize.x),
               static_cast<int>(io.DisplaySize.y));
    glClearColor(0, 0, 0, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    cen::gl::swap(mWindow);
  }

  mWindow.hide();
  return 0;
}

void Application::UpdateFrame()
{
  ShowGui();
}

}  // namespace tactile

// Copyright (C) 2025 Albin Johansson

module;

#include <imgui.h>
#include <imgui_impl_sdl3.h>

#ifdef TACTILE_ENABLE_OPENGL
  #include <imgui_impl_opengl3.h>
#endif

export module tactile.ui:imgui;

// NOLINTBEGIN(*-unused-using-decls)

export namespace ImGui {

using ImGui::Begin;
using ImGui::CreateContext;
using ImGui::DestroyContext;
using ImGui::End;
using ImGui::GetDrawData;
using ImGui::GetIO;
using ImGui::NewFrame;
using ImGui::Render;
using ImGui::ShowDemoWindow;

}  // namespace ImGui

export {
  using ::ImGuiContext;
  using ::ImVec2;
  using ::ImVec4;

  using ::ImGui_ImplSDL3_InitForOpenGL;
  using ::ImGui_ImplSDL3_InitForVulkan;
  using ::ImGui_ImplSDL3_NewFrame;
  using ::ImGui_ImplSDL3_ProcessEvent;
  using ::ImGui_ImplSDL3_Shutdown;

#ifdef TACTILE_ENABLE_OPENGL
  using ::ImGui_ImplOpenGL3_CreateDeviceObjects;
  using ::ImGui_ImplOpenGL3_CreateFontsTexture;
  using ::ImGui_ImplOpenGL3_DestroyDeviceObjects;
  using ::ImGui_ImplOpenGL3_DestroyFontsTexture;
  using ::ImGui_ImplOpenGL3_Init;
  using ::ImGui_ImplOpenGL3_NewFrame;
  using ::ImGui_ImplOpenGL3_RenderDrawData;
  using ::ImGui_ImplOpenGL3_Shutdown;
#endif
}

// NOLINTEND(*-unused-using-decls)

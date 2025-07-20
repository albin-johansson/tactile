// Copyright (C) 2025 Albin Johansson

module;

#include <imgui.h>
#include <imgui_impl_sdl3.h>

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

  using ::ImGui_ImplSDL3_NewFrame;
  using ::ImGui_ImplSDL3_ProcessEvent;
  using ::ImGui_ImplSDL3_Shutdown;
}

// NOLINTEND(*-unused-using-decls)

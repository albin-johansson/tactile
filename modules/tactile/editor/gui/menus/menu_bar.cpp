#include "menu_bar.hpp"

#include <imgui.h>

#include "editor/model.hpp"
#include "map_menu.hpp"

namespace tactile {

void MenuBar::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    mFileMenu.Update(model, dispatcher);
    mEditMenu.Update(model, dispatcher);
    mViewMenu.Update(model, dispatcher);
    mMapMenu.Update(model, dispatcher);
    mHelpMenu.Update(model, dispatcher);
    mDebugMenu.Update();

    ImGui::EndMainMenuBar();
  }

  mFileMenu.UpdateWindows(model, dispatcher);
  mEditMenu.UpdateWindows(model, dispatcher);
  mHelpMenu.UpdateWindows();
  mDebugMenu.UpdateWindows();
}

void MenuBar::ShowSettings()
{
  mEditMenu.OpenSettingsModal();
}

void MenuBar::ShowNewMapDialog()
{
  mFileMenu.ShowNewMapDialog();
}

void MenuBar::ShowOpenMapDialog()
{
  mFileMenu.ShowOpenMapDialog();
}

void MenuBar::ShowAddTilesetDialog()
{
  mMapMenu.ShowAddTilesetDialog();
}

void MenuBar::ShowComponentEditor(const Model& model)
{
  mEditMenu.ShowComponentEditor(model);
}

}  // namespace tactile

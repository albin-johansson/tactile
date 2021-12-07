#include "menu_bar.hpp"

#include <imgui.h>

#include "map_menu.hpp"
#include "editor/model.hpp"

namespace Tactile {

void MenuBar::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    mFileMenu.Update(model, dispatcher);
    mEditMenu.Update(model, dispatcher);
    mViewMenu.Update(model, dispatcher);
    mMapMenu.Update(model, dispatcher);
    mHelpMenu.Update();
    mDebugMenu.Update();

    ImGui::EndMainMenuBar();
  }

  if (const auto* registry = model.GetActiveRegistry()) {
    mFileMenu.UpdateWindows(*registry, dispatcher);
  }

  mEditMenu.UpdateWindows(dispatcher);
  mMapMenu.UpdateWindows(dispatcher);
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

}  // namespace Tactile

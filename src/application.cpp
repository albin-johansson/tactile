#include "application.hpp"

#include <imgui.h>

#include <utility>  // move

#include "application_events.hpp"
#include "gui/cursors.hpp"
#include "gui/icons.hpp"
#include "gui/update_gui.hpp"
#include "gui/widgets/viewport/viewport_widget.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "io/preferences.hpp"
#include "shortcuts.hpp"
#include "utils/load_texture.hpp"

namespace Tactile {

Application::Application(cen::window&& window)
    : mWindow{std::move(window)}
    , mModel{std::make_unique<Model>()}
{
  SubscribeToEvents(this, mDispatcher);
  LoadCursors();
  LoadIcons();

  OnAddMapEvent();

  auto& map = mModel->GetActiveMap();
  map.AddTileLayer();
  map.AddObjectLayer();
  map.SelectLayer(1_layer);

  auto* document = mModel->GetActiveDocument();

  using namespace std::string_literals;
  document->AddProperty("Aa", "Foo"s);
  document->AddProperty("Ab", "Bar"s);

  document->AddProperty("Ba", 123);
  document->AddProperty("Bb", 1337);

  document->AddProperty("Ca", 12.3f);
  document->AddProperty("Cb", 42.5f);

  document->AddProperty("Da", true);
  document->AddProperty("Db", false);

  document->AddProperty("Ea", std::filesystem::path{"foo.json"});
  document->AddProperty("Eb", std::filesystem::path{"bar.json"});

  document->AddProperty("Fa", object_ref{42});
  document->AddProperty("Fb", object_ref{12});

  document->AddProperty("Ga", cen::colors::magenta);
  document->AddProperty("Gb", cen::colors::cyan);
}

auto Application::Run() -> int
{
  const auto& io = ImGui::GetIO();

  while (!mQuit)
  {
    PollEvents();

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
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    cen::gl::swap(mWindow);
  }

  OnAboutToExit();

  mWindow.hide();
  return 0;
}

void Application::OnAboutToExit()
{
  UnloadIcons();
  UnloadCursors();
  SavePreferences();

  for (const auto texture : mTextures)
  {
    CENTURION_LOG_DEBUG("Deleting texture %u...", texture);
    glDeleteTextures(1, &texture);
  }
}

void Application::PollEvents()
{
  cen::event event;
  while (event.poll())
  {
    ImGui_ImplSDL2_ProcessEvent(event.data());

    if (event.is<cen::quit_event>())
    {
      mQuit = true;
      break;
    }
    else if (const auto* keyEvent = event.try_get<cen::keyboard_event>())
    {
      UpdateShortcuts(*mModel, *keyEvent, mDispatcher);
    }
  }
}

void Application::UpdateFrame()
{
  mDispatcher.update();
  UpdateGui(*mModel, mDispatcher);
}

void Application::OnAddMapEvent()
{
  const auto id = mModel->AddMap();
  mModel->SelectMap(id);
}

void Application::OnCloseMapEvent(const CloseMapEvent& event)
{
  mModel->RemoveMap(event.id);
}

void Application::OnOpenMapEvent(const OpenMapEvent& event)
{
  // TODO
}

void Application::OnAddTilesetEvent(const AddTilesetEvent& event)
{
  if (const auto info = LoadTexture(event.path))
  {
    CENTURION_LOG_DEBUG("Loaded texture with ID %u", info->texture);
    mTextures.push_back(info->texture);

    if (auto* document = mModel->GetActiveDocument())
    {
      document->AddTileset(*info, event.tile_width, event.tile_height);
    }
  }
  else
  {
    CENTURION_LOG_WARN("Failed to load tileset texture!");
  }
}

void Application::OnUndoEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->Undo();
  }
}

void Application::OnRedoEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->Redo();
  }
}

void Application::OnSelectToolEvent(const SelectToolEvent& event)
{
  mModel->SelectTool(event.type);
}

void Application::OnMousePressedEvent(const MousePressedEvent& event)
{
  mModel->OnMousePressed(event);
}

void Application::OnMouseReleasedEvent(const MouseReleasedEvent& event)
{
  mModel->OnMouseReleased(event);
}

void Application::OnMouseDragEvent(const MouseDragEvent& event)
{
  mModel->OnMouseDragged(event);
}

void Application::OnCenterViewportEvent()
{
  CenterViewport();
}

void Application::OnSelectMapEvent(const SelectMapEvent& event)
{
  mModel->SelectMap(event.id);
}

void Application::OnSelectTilesetEvent(const SelectTilesetEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->SelectTileset(event.id);
  }
}

void Application::OnRemoveTilesetEvent(const RemoveTilesetEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->RemoveTileset(event.id);
  }
}

void Application::OnAddRowEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->AddRow();
  }
}

void Application::OnAddColumnEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->AddColumn();
  }
}

void Application::OnRemoveRowEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->RemoveRow();
  }
}

void Application::OnRemoveColumnEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->RemoveColumn();
  }
}

void Application::OnAddLayerEvent(const AddLayerEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    switch (event.type)
    {
      case LayerType::TileLayer:
        document->AddTileLayer();
        break;

      case LayerType::ObjectLayer:
        document->AddObjectLayer();
        break;
    }
  }
}

void Application::OnRemoveLayerEvent(const RemoveLayerEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->RemoveLayer(event.id);
  }
}

void Application::OnMoveLayerUpEvent(const MoveLayerUpEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->MoveLayerUp(event.id);
  }
}

void Application::OnMoveLayerDownEvent(const MoveLayerDownEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->MoveLayerDown(event.id);
  }
}

void Application::OnDuplicateLayerEvent(const DuplicateLayerEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->DuplicateLayer(event.id);
  }
}

void Application::OnSetLayerOpacityEvent(const SetLayerOpacityEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->SetLayerOpacity(event.id, event.opacity);
  }
}

void Application::OnSetLayerVisibleEvent(const SetLayerVisibleEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->SetLayerVisible(event.id, event.visible);
  }
}

void Application::OnShowLayerPropertiesEvent(const ShowLayerPropertiesEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->ShowLayerProperties(event.id);
  }
}

void Application::OnShowMapPropertiesEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->ShowProperties();
  }
}

void Application::OnSelectLayerEvent(const SelectLayerEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->SelectLayer(event.id);
  }
}

void Application::OnAddPropertyEvent(const AddPropertyEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->AddProperty(event.name, event.type);
  }
}

void Application::OnRemovePropertyEvent(const RemovePropertyEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->RemoveProperty(event.name);
  }
}

void Application::OnRenamePropertyEvent(const RenamePropertyEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->RenameProperty(event.old_name, event.new_name);
  }
}

void Application::OnSetPropertyValueEvent(const SetPropertyValueEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->SetProperty(event.name, event.property);
  }
}

void Application::OnChangePropertyTypeEvent(const ChangePropertyTypeEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->ChangePropertyType(event.name, event.type);
  }
}

void Application::OnSetTilesetSelectionEvent(const SetTilesetSelectionEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    auto& tilesets = document->GetTilesets();
    if (auto* tileset = tilesets.GetActiveTileset())
    {
      tileset->SetSelection(event.selection);
    }
  }
}

void Application::OnQuitEvent()
{
  mQuit = true;
}

}  // namespace Tactile

#include "application.hpp"

#include <utility>  // move

#include "gui/update_gui.hpp"
#include "gui/widgets/menus/menu_bar_widget.hpp"
#include "gui/widgets/viewport/viewport_widget.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "utils/load_texture.hpp"

namespace tactile {

Application::Application(cen::window&& window, cen::gl_context&& context)
    : mWindow{std::move(window)}
    , mContext{std::move(context)}
    , mModel{std::make_unique<Model>()}
{
  // clang-format off
  mDispatcher.sink<UndoEvent>().connect<&Application::OnUndoEvent>(this);
  mDispatcher.sink<RedoEvent>().connect<&Application::OnRedoEvent>(this);

  mDispatcher.sink<AddMapEvent>().connect<&Application::OnAddMapEvent>(this);
  mDispatcher.sink<CloseMapEvent>().connect<&Application::OnCloseMapEvent>(this);
  mDispatcher.sink<OpenMapEvent>().connect<&Application::OnOpenMapEvent>(this);

  mDispatcher.sink<AddTilesetEvent>().connect<&Application::OnAddTilesetEvent>(this);

  mDispatcher.sink<CenterViewportEvent>().connect<&Application::OnCenterViewportEvent>(this);
  mDispatcher.sink<SelectLayerEvent>().connect<&Application::OnSelectLayerEvent>(this);
  mDispatcher.sink<SelectMapEvent>().connect<&Application::OnSelectMapEvent>(this);

  mDispatcher.sink<AddRowEvent>().connect<&Application::OnAddRowEvent>(this);
  mDispatcher.sink<AddColumnEvent>().connect<&Application::OnAddColumnEvent>(this);
  mDispatcher.sink<RemoveRowEvent>().connect<&Application::OnRemoveRowEvent>(this);
  mDispatcher.sink<RemoveColumnEvent>().connect<&Application::OnRemoveColumnEvent>(this);

  mDispatcher.sink<SetPropertyValueEvent>().connect<&Application::OnSetPropertyValueEvent>(this);

  mDispatcher.sink<QuitEvent>().connect<&Application::OnQuitEvent>(this);
  // clang-format on

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

Application::~Application()
{
  for (const auto texture : mTextures)
  {
    glDeleteTextures(1, &texture);
  }
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

  mWindow.hide();
  return 0;
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
      if (keyEvent->pressed())
      {
        const auto ctrl = keyEvent->is_active(cen::key_modifier::ctrl);
        const auto shift = keyEvent->is_active(cen::key_modifier::shift);
        const auto alt = keyEvent->is_active(cen::key_modifier::alt);

        const auto key = keyEvent->scan();
        if (ctrl && alt)
        {
          OnCtrlAltKeyStroke(key);
        }
        else if (alt && shift)
        {
          OnAltShiftKeyStroke(key);
        }
        else if (ctrl)
        {
          OnCtrlKeyStroke(key);
        }
        else if (alt)
        {
          OnAltKeyStroke(key);
        }
      }
    }
  }
}

void Application::UpdateFrame()
{
  mDispatcher.update();
  UpdateGui(*mModel, mDispatcher);
}

void Application::OnCtrlAltKeyStroke(const cen::scan_code key)
{
  if (key == cen::scancodes::s)
  {
    EnableSettingsDialog();
  }
}

void Application::OnAltShiftKeyStroke(const cen::scan_code key)
{
  if (key == cen::scancodes::r)
  {
    OnRemoveRowEvent();
  }
  else if (key == cen::scancodes::c)
  {
    OnRemoveColumnEvent();
  }
}

void Application::OnCtrlKeyStroke(const cen::scan_code key)
{
  if (key == cen::scancodes::n)
  {
    OnAddMapEvent();
  }
  else if (key == cen::scancodes::g)
  {
    ToggleMapGrid();
  }
  else if (key == cen::scancodes::o)
  {
    EnableOpenMapDialog();
  }
  else if (key == cen::scancodes::t)
  {
    EnableTilesetDialog();
  }
  else if (key == cen::scancodes::space)
  {
    OnCenterViewportEvent();
  }
}

void Application::OnAltKeyStroke(const cen::scan_code key)
{
  if (key == cen::scancodes::r)
  {
    OnAddRowEvent();
  }
  else if (key == cen::scancodes::c)
  {
    OnAddColumnEvent();
  }
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
  cen::log::info("Application::OnOpenMapEvent > %s",
                 event.path.filename().string().c_str());
}

void Application::OnAddTilesetEvent(const AddTilesetEvent& event)
{
  if (const auto info = LoadTexture(event.path))
  {
    mTextures.push_back(info->texture);

    // TODO add tileset to model
  }
}

void Application::OnUndoEvent(const UndoEvent& event)
{
  cen::log::info("Application::OnUndoEvent");
}

void Application::OnRedoEvent(const RedoEvent& event)
{
  cen::log::info("Application::OnRedoEvent");
}

void Application::OnCenterViewportEvent()
{
  CenterViewport();
}

void Application::OnSelectMapEvent(const SelectMapEvent& event)
{
  mModel->SelectMap(event.id);
}

void Application::OnSelectLayerEvent(const SelectLayerEvent& event)
{
  auto& map = mModel->GetActiveMap();
  map.SelectLayer(event.id);
  CENTURION_LOG_DEBUG("Selected layer %i!", event.id.get());
}

void Application::OnAddRowEvent()
{
  auto& map = mModel->GetActiveMap();
  map.AddRow(empty_tile);
}

void Application::OnAddColumnEvent()
{
  auto& map = mModel->GetActiveMap();
  map.AddColumn(empty_tile);
}

void Application::OnRemoveRowEvent()
{
  auto& map = mModel->GetActiveMap();
  map.RemoveRow();
}

void Application::OnRemoveColumnEvent()
{
  auto& map = mModel->GetActiveMap();
  map.RemoveColumn();
}

void Application::OnSetPropertyValueEvent(const SetPropertyValueEvent& event)
{
  auto* document = mModel->GetActiveDocument();
  document->SetProperty(event.name, event.property);
}

void Application::OnQuitEvent()
{
  mQuit = true;
}

}  // namespace tactile

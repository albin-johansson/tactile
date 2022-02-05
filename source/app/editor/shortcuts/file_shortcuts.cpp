#include "file_shortcuts.hpp"

#include "editor/events/edit_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/save_events.hpp"
#include "editor/gui/menus/edit_menu.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "mappings.hpp"

namespace tactile {

NewMapShortcut::NewMapShortcut() : AShortcut{cen::scancodes::n, gPrimaryModifier} {}

void NewMapShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<show_new_map_dialog_event>();
}

OpenMapShortcut::OpenMapShortcut() : AShortcut{cen::scancodes::o, gPrimaryModifier} {}

void OpenMapShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<show_open_map_dialog_event>();
}

OpenSettingsShortcut::OpenSettingsShortcut()
    : AShortcut{SDL_SCANCODE_COMMA, gPrimaryModifier}
{}

void OpenSettingsShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<show_settings_event>();
}

SaveShortcut::SaveShortcut() : AShortcut{cen::scancodes::s, gPrimaryModifier} {}

void SaveShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<save_event>();
}

auto SaveShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.CanSaveDocument();
}

SaveAsShortcut::SaveAsShortcut()
    : AShortcut{cen::scancodes::s, gPrimaryModifier | cen::key_mod::lshift}
{}

void SaveAsShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<open_save_as_dialog_event>();
}

auto SaveAsShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.CanSaveDocument();
}

}  // namespace tactile

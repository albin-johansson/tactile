#include "file_shortcuts.hpp"

#include "editor/events/edit_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/save_events.hpp"
#include "editor/gui/menus/edit_menu.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "mappings.hpp"

namespace Tactile {

NewMapShortcut::NewMapShortcut() : AShortcut{cen::scancodes::n, gPrimaryModifier} {}

void NewMapShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowNewMapDialogEvent>();
}

OpenMapShortcut::OpenMapShortcut() : AShortcut{cen::scancodes::o, gPrimaryModifier} {}

void OpenMapShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowOpenMapDialogEvent>();
}

OpenSettingsShortcut::OpenSettingsShortcut()
    : AShortcut{SDL_SCANCODE_COMMA, gPrimaryModifier}
{}

void OpenSettingsShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowSettingsEvent>();
}

SaveShortcut::SaveShortcut() : AShortcut{cen::scancodes::s, gPrimaryModifier} {}

void SaveShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SaveEvent>();
}

auto SaveShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.CanSaveDocument();
}

SaveAsShortcut::SaveAsShortcut()
    : AShortcut{cen::scancodes::s, gPrimaryModifier | KMOD_LSHIFT}
{}

void SaveAsShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<OpenSaveAsDialogEvent>();
}

auto SaveAsShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.CanSaveDocument();
}

}  // namespace Tactile

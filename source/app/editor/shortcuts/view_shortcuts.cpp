#include "view_shortcuts.hpp"

#include "editor/events/misc_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"
#include "mappings.hpp"

namespace tactile {

CenterViewportShortcut::CenterViewportShortcut()
    : AShortcut{cen::scancodes::space, cen::key_mod::lshift}
{}

void CenterViewportShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CenterViewportEvent>();
}

auto CenterViewportShortcut::IsEnabled(const document_model& model,
                                       const widget_manager&) const -> bool
{
  return model.has_active_document();
}

DecreaseViewportZoomShortcut::DecreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_MINUS}, gPrimaryModifier}
{}

void DecreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<DecreaseZoomEvent>();
}

auto DecreaseViewportZoomShortcut::IsEnabled(const document_model& model,
                                             const widget_manager&) const -> bool
{
  return model.can_decrease_viewport_tile_size();
}

IncreaseViewportZoomShortcut::IncreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_PLUS}, gPrimaryModifier}
{}

void IncreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<IncreaseZoomEvent>();
}

auto IncreaseViewportZoomShortcut::IsEnabled(const document_model& model,
                                             const widget_manager&) const -> bool
{
  return model.has_active_document();
}

PanUpShortcut::PanUpShortcut()
    : AShortcut{cen::scancodes::up, gPrimaryModifier | cen::key_mod::lshift}
{}

void PanUpShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanUpEvent>();
}

auto PanUpShortcut::IsEnabled(const document_model& model, const widget_manager&) const
    -> bool
{
  return model.has_active_document();
}

PanDownShortcut::PanDownShortcut()
    : AShortcut{cen::scancodes::down, gPrimaryModifier | cen::key_mod::lshift}
{}

void PanDownShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanDownEvent>();
}

auto PanDownShortcut::IsEnabled(const document_model& model, const widget_manager&) const
    -> bool
{
  return model.has_active_document();
}

PanLeftShortcut::PanLeftShortcut()
    : AShortcut{cen::scancodes::left, gPrimaryModifier | cen::key_mod::lshift}
{}

void PanLeftShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanLeftEvent>();
}

auto PanLeftShortcut::IsEnabled(const document_model& model, const widget_manager&) const
    -> bool
{
  return model.has_active_document();
}

PanRightShortcut::PanRightShortcut()
    : AShortcut{cen::scancodes::right, gPrimaryModifier | cen::key_mod::lshift}
{}

void PanRightShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanRightEvent>();
}

auto PanRightShortcut::IsEnabled(const document_model& model, const widget_manager&) const
    -> bool
{
  return model.has_active_document();
}

ToggleGridShortcut::ToggleGridShortcut() : AShortcut{cen::scancodes::g, gPrimaryModifier}
{}

void ToggleGridShortcut::Activate(entt::dispatcher&)
{
  auto& prefs = get_preferences();
  prefs.set_grid_visible(!prefs.is_grid_visible());
}

ToggleUiShortcut::ToggleUiShortcut() : AShortcut{cen::scancodes::tab} {}

void ToggleUiShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<toggle_ui_event>();
}

auto ToggleUiShortcut::IsEnabled(const document_model& model,
                                 const widget_manager& widgets) const -> bool
{
  return model.has_active_document() && widgets.is_editor_focused();
}

}  // namespace tactile
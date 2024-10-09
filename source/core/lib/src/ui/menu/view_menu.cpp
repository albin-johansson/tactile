// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/menu/view_menu.hpp"

#include <imgui.h>

#include "tactile/core/document/document_info.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/view_events.hpp"
#include "tactile/core/event/viewport_events.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/menus.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/shortcuts.hpp"

namespace tactile::core::ui {

void ViewMenu::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const auto* document = model.get_current_document();

  const auto root_entity = (document != nullptr)
                               ? document->get_registry().get<CDocumentInfo>().root
                               : kInvalidEntity;
  const auto content_size = (document != nullptr) ? document->get_content_size() : Float2 {};

  if (const MenuScope menu {language.get(StringID::kViewMenu)}; menu.is_open()) {
    _push_widgets_menu(language, dispatcher);

    if (ImGui::MenuItem(language.get(StringID::kToggleUi))) {
      dispatcher.push<ToggleUiEvent>();
    }

    ImGui::Separator();

    _push_theme_menu(language, dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kIncreaseFontSize),
                        kIncreaseFontSizeShortcut.hint)) {
      dispatcher.push<IncreaseFontSizeEvent>();
    }

    if (ImGui::MenuItem(language.get(StringID::kDecreaseFontSize),
                        kDecreaseFontSizeShortcut.hint)) {
      dispatcher.push<DecreaseFontSizeEvent>();
    }

    if (ImGui::MenuItem(language.get(StringID::kResetFontSize))) {
      dispatcher.push<ResetFontSizeEvent>();
    }

    _push_font_menu(language, dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kIncreaseZoom),
                        kIncreaseZoomShortcut.hint,
                        false,
                        document != nullptr)) {
      dispatcher.push<IncreaseViewportZoomEvent>(root_entity);
    }

    if (ImGui::MenuItem(language.get(StringID::kDecreaseZoom),
                        kDecreaseZoomShortcut.hint,
                        false,
                        document != nullptr)) {
      dispatcher.push<DecreaseViewportZoomEvent>(root_entity);
    }

    if (ImGui::MenuItem(language.get(StringID::kResetZoom),
                        nullptr,
                        false,
                        document != nullptr)) {
      dispatcher.push<ResetViewportZoomEvent>(root_entity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kCenterViewport),
                        kCenterViewportShortcut.hint,
                        false,
                        document != nullptr)) {
      dispatcher.push<CenterViewportEvent>(root_entity, content_size);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kToggleGrid), kToggleGridShortcut.hint)) {
      dispatcher.push<ToggleGridEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kPanUp),
                        kPanUpShortcut.hint,
                        false,
                        document != nullptr)) {
      dispatcher.push<PanViewportUpEvent>(root_entity);
    }

    if (ImGui::MenuItem(language.get(StringID::kPanDown),
                        kPanDownShortcut.hint,
                        false,
                        document != nullptr)) {
      dispatcher.push<PanViewportDownEvent>(root_entity);
    }

    if (ImGui::MenuItem(language.get(StringID::kPanLeft),
                        kPanLeftShortcut.hint,
                        false,
                        document != nullptr)) {
      dispatcher.push<PanViewportLeftEvent>(root_entity);
    }

    if (ImGui::MenuItem(language.get(StringID::kPanRight),
                        kPanRightShortcut.hint,
                        false,
                        document != nullptr)) {
      dispatcher.push<PanViewportRightEvent>(root_entity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kHighlightActiveLayer))) {
      dispatcher.push<ToggleLayerHighlightEvent>();
    }
  }
}

void ViewMenu::_push_widgets_menu(const Language& language, EventDispatcher& dispatcher)
{
  if (const MenuScope widgets_menu {language.get(StringID::kWidgetsMenu)};
      widgets_menu.is_open()) {
    if (ImGui::MenuItem(language.get(StringID::kResetLayout))) {
      dispatcher.push<ResetLayoutEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kPropertyDock), nullptr, true)) {
      dispatcher.push<TogglePropertyDockEvent>();
    }

    if (ImGui::MenuItem(language.get(StringID::kComponentDock), nullptr, true)) {
      dispatcher.push<ToggleComponentDockEvent>();
    }

    if (ImGui::MenuItem(language.get(StringID::kLayerDock), nullptr, true)) {
      dispatcher.push<ToggleLayerDockEvent>();
    }

    if (ImGui::MenuItem(language.get(StringID::kTilesetDock), nullptr, true)) {
      dispatcher.push<ToggleTilesetDockEvent>();
    }

    if (ImGui::MenuItem(language.get(StringID::kAnimationDock), nullptr, true)) {
      dispatcher.push<ToggleAnimationDockEvent>();
    }

    if (ImGui::MenuItem(language.get(StringID::kLogDock), nullptr, true)) {
      dispatcher.push<ToggleLogDockEvent>();
    }
  }
}

void ViewMenu::_push_font_menu(const Language& language, EventDispatcher& dispatcher)
{
  if (const MenuScope font_menu {language.get(StringID::kFont)}; font_menu.is_open()) {
    if (ImGui::MenuItem(language.get(StringID::kDefault))) {
      dispatcher.push<SetFontEvent>(FontID::kDefault);
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Roboto")) {
      dispatcher.push<SetFontEvent>(FontID::kRoboto);
    }
  }
}

void ViewMenu::_push_theme_menu(const Language& language, EventDispatcher& dispatcher)
{
  if (const MenuScope theme_menu {language.get(StringID::kThemeMenu)}; theme_menu.is_open()) {
    if (ImGui::MenuItem("Dear ImGui##Light")) {
      ImGui::StyleColorsLight(&ImGui::GetStyle());
      dispatcher.push<SetThemeEvent>(Theme::kDearImGuiLight);
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Dear ImGui##Dark")) {
      ImGui::StyleColorsDark(&ImGui::GetStyle());
      dispatcher.push<SetThemeEvent>(Theme::kDearImGuiDark);
    }
  }
}

}  // namespace tactile::core::ui

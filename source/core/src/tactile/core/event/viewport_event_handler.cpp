// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/event/viewport_event_handler.hpp"

#include <imgui.h>

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/viewport_events.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/model/settings.hpp"
#include "tactile/core/numeric/vec_format.hpp"
#include "tactile/core/ui/imgui_compat.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile {

ViewportEventHandler::ViewportEventHandler(Model* model)
  : mModel {require_not_null(model, "null model")}
{}

void ViewportEventHandler::install(EventDispatcher& dispatcher)
{
  TACTILE_LOG_DEBUG("Installing viewport event handler");

  using Self = ViewportEventHandler;

  // clang-format off
  dispatcher.bind<OffsetViewportEvent, &Self::on_offset_viewport>(this);
  dispatcher.bind<UpdateViewportSizeEvent, &Self::on_update_viewport_size>(this);
  dispatcher.bind<IncreaseViewportZoomEvent, &Self::on_increase_viewport_zoom>(this);
  dispatcher.bind<DecreaseViewportZoomEvent, &Self::on_decrease_viewport_zoom>(this);
  dispatcher.bind<ResetViewportZoomEvent, &Self::on_reset_viewport_zoom>(this);
  dispatcher.bind<CenterViewportEvent, &Self::on_center_viewport>(this);
  // clang-format on
}

void ViewportEventHandler::on_offset_viewport(const OffsetViewportEvent& event)
{
  if (mModel->get_settings().log_verbose_events) {
    TACTILE_LOG_TRACE("OffsetViewportEvent(viewport: {}, offset: {})",
                      entity_to_string(event.viewport_entity),
                      event.delta);
  }

  if (auto* document = mModel->get_current_document()) {
    auto& registry = document->get_registry();

    const auto root_entity = document->get_root_entity();
    translate_viewport(registry, root_entity, event.delta);
  }
}

void ViewportEventHandler::on_update_viewport_size(
    const UpdateViewportSizeEvent& event)
{
  if (mModel->get_settings().log_verbose_events) {
    TACTILE_LOG_TRACE("UpdateViewportSizeEvent(viewport: {}, size: {})",
                      entity_to_string(event.viewport_entity),
                      event.new_size);
  }

  if (auto* document = mModel->get_current_document()) {
    auto& registry = document->get_registry();
    auto& viewport = registry.get<CViewport>(event.viewport_entity);
    viewport.size = event.new_size;
  }
}

void ViewportEventHandler::on_increase_viewport_zoom(
    const IncreaseViewportZoomEvent& event)
{
  if (mModel->get_settings().log_verbose_events) {
    TACTILE_LOG_TRACE("IncreaseViewportZoomEvent(viewport: {})",
                      entity_to_string(event.viewport_entity));
  }

  if (auto* document = mModel->get_current_document()) {
    auto& registry = document->get_registry();
    increase_viewport_zoom(registry,
                           event.viewport_entity,
                           to_float2(ImGui::GetMousePos()));
  }
}

void ViewportEventHandler::on_decrease_viewport_zoom(
    const DecreaseViewportZoomEvent& event)
{
  if (mModel->get_settings().log_verbose_events) {
    TACTILE_LOG_TRACE("DecreaseViewportZoomEvent(viewport: {})",
                      entity_to_string(event.viewport_entity));
  }

  if (auto* document = mModel->get_current_document()) {
    auto& registry = document->get_registry();
    decrease_viewport_zoom(registry,
                           event.viewport_entity,
                           to_float2(ImGui::GetMousePos()));
  }
}

void ViewportEventHandler::on_reset_viewport_zoom(
    const ResetViewportZoomEvent& event)
{
  TACTILE_LOG_TRACE("ResetViewportZoomEvent(viewport: {})",
                    entity_to_string(event.viewport_entity));

  if (auto* document = mModel->get_current_document()) {
    auto& registry = document->get_registry();
    reset_viewport_zoom(registry, event.viewport_entity);
  }
}

void ViewportEventHandler::on_center_viewport(const CenterViewportEvent& event)
{
  TACTILE_LOG_TRACE("CenterViewportEvent(viewport: {}, content_size: {})",
                    entity_to_string(event.viewport_entity),
                    event.content_size);

  if (auto* document = mModel->get_current_document()) {
    auto& registry = document->get_registry();
    center_viewport_over_content(registry,
                                 event.viewport_entity,
                                 event.content_size);
  }
}

}  // namespace tactile

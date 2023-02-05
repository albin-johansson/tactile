/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "common/debug/assert.hpp"
#include "common/type/math.hpp"
#include "common/util/fmt.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/viewport.hpp"
#include "handlers/event_handlers.hpp"
#include "model/document/map_document.hpp"
#include "model/event/viewport_events.hpp"
#include "model/model.hpp"
#include "runtime/app_context.hpp"
#include "ui/conversions.hpp"
#include "ui/viewport/map_viewport.hpp"
#include "ui/viewport/tileset_viewport.hpp"

namespace tactile {
namespace {

inline constexpr Float2 kViewportDefaultTileSize = {64, 64};

void on_viewport_mouse_pressed(const ViewportMousePressedEvent& event)
{
  spdlog::trace("ViewportMousePressedEvent(...)");

  auto& model = get_model();
  if (auto* map_document = model.active_map_document()) {
    auto& tool_manager = map_document->get_tools();
    tool_manager.on_pressed(model, get_dispatcher(), event.info);
  }
}

void on_viewport_mouse_dragged(const ViewportMouseDraggedEvent& event)
{
  spdlog::trace("ViewportMouseDraggedEvent(...)");

  auto& model = get_model();
  if (auto* map_document = model.active_map_document()) {
    auto& tool_manager = map_document->get_tools();
    tool_manager.on_dragged(model, get_dispatcher(), event.info);
  }
}

void on_viewport_mouse_released(const ViewportMouseReleasedEvent& event)
{
  spdlog::trace("ViewportMouseReleasedEvent(...)");

  auto& model = get_model();
  if (auto* map_document = model.active_map_document()) {
    auto& tool_manager = map_document->get_tools();
    tool_manager.on_released(model, get_dispatcher(), event.info);
  }
}

void on_viewport_mouse_entered(const ViewportMouseEnteredEvent&)
{
  spdlog::trace("ViewportMouseEnteredEvent(...)");

  auto& model = get_model();
  if (auto* map_document = model.active_map_document()) {
    auto& tool_manager = map_document->get_tools();
    tool_manager.on_entered(model, get_dispatcher());
  }
}

void on_viewport_mouse_exited(const ViewportMouseExitedEvent&)
{
  spdlog::trace("ViewportMouseExitedEvent(...)");

  auto& model = get_model();
  if (auto* map_document = model.active_map_document()) {
    auto& tool_manager = map_document->get_tools();
    tool_manager.on_exited(model, get_dispatcher());
  }
}

void on_pan_left(const PanLeftEvent&)
{
  spdlog::trace("PanLeftEvent");

  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_left();
  }
}

void on_pan_right(const PanRightEvent&)
{
  spdlog::trace("PanRightEvent");

  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_right();
  }
}

void on_pan_up(const PanUpEvent&)
{
  spdlog::trace("PanUpEvent");

  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_up();
  }
}

void on_pan_down(const PanDownEvent&)
{
  spdlog::trace("PanDownEvent");

  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_down();
  }
}

void on_reset_zoom(const ResetZoomEvent&)
{
  spdlog::trace("ResetZoomEvent");

  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.set_tile_size(kViewportDefaultTileSize);
  }
}

void on_increase_zoom(const IncreaseZoomEvent&)
{
  spdlog::trace("IncreaseZoomEvent");

  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.zoom_in(as_float2(ImGui::GetIO().MousePos));
  }
}

void on_decrease_zoom(const DecreaseZoomEvent&)
{
  spdlog::trace("DecreaseZoomEvent");

  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.zoom_out(as_float2(ImGui::GetIO().MousePos));
  }
}

void on_center_viewport(const CenterViewportEvent&)
{
  spdlog::trace("CenterViewportEvent");

  TACTILE_ASSERT(get_model().is_map_active() || get_model().is_tileset_active());

  if (get_model().is_map_active()) {
    ui::center_map_viewport();
  }
  else {
    ui::center_tileset_viewport();
  }
}

void on_offset_document_viewport(const OffsetDocumentViewportEvent& event)
{
  spdlog::trace("OffsetDocumentViewportEvent(delta: {})", event.delta);

  // FIXME possible crash if tileset is removed when offsetting viewport
  if (auto* document = get_model().active_document()) {
    auto& viewport = document->get_viewport();
    viewport.offset(event.delta);
  }
}

void on_offset_tileset_viewport(const OffsetTilesetViewportEvent& event)
{
  spdlog::trace("OffsetTilesetViewportEvent(tileset_id: {}, delta: {})",
                event.tileset_id,
                event.delta);

  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    auto& tileset_ref =
        document->get_map().get_tileset_bundle().get_tileset_ref(event.tileset_id);
    auto& viewport = tileset_ref.get_viewport();
    viewport.offset(event.delta);
  }
}

void on_update_tileset_viewport_limits(const UpdateTilesetViewportLimitsEvent& event)
{
  spdlog::trace(
      "UpdateTilesetViewportLimitsEvent(tileset_id: {}, min_offset: {}, max_offset: {})",
      event.tileset_id,
      event.min_offset,
      event.max_offset);

  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    auto& tileset_ref =
        document->get_map().get_tileset_bundle().get_tileset_ref(event.tileset_id);
    auto& viewport = tileset_ref.get_viewport();
    viewport.set_limits({event.min_offset, event.max_offset});
  }
}

}  // namespace

void install_viewport_event_handler()
{
  auto& dispatcher = get_dispatcher();

  dispatcher.sink<ViewportMousePressedEvent>().connect<&on_viewport_mouse_pressed>();
  dispatcher.sink<ViewportMouseDraggedEvent>().connect<&on_viewport_mouse_dragged>();
  dispatcher.sink<ViewportMouseReleasedEvent>().connect<&on_viewport_mouse_released>();
  dispatcher.sink<ViewportMouseEnteredEvent>().connect<&on_viewport_mouse_entered>();
  dispatcher.sink<ViewportMouseExitedEvent>().connect<&on_viewport_mouse_exited>();

  dispatcher.sink<PanLeftEvent>().connect<&on_pan_left>();
  dispatcher.sink<PanRightEvent>().connect<&on_pan_right>();
  dispatcher.sink<PanUpEvent>().connect<&on_pan_up>();
  dispatcher.sink<PanDownEvent>().connect<&on_pan_down>();

  dispatcher.sink<ResetZoomEvent>().connect<&on_reset_zoom>();
  dispatcher.sink<IncreaseZoomEvent>().connect<&on_increase_zoom>();
  dispatcher.sink<DecreaseZoomEvent>().connect<&on_decrease_zoom>();

  dispatcher.sink<CenterViewportEvent>().connect<&on_center_viewport>();

  dispatcher.sink<OffsetDocumentViewportEvent>().connect<&on_offset_document_viewport>();
  dispatcher.sink<OffsetTilesetViewportEvent>().connect<&on_offset_tileset_viewport>();
  dispatcher.sink<UpdateTilesetViewportLimitsEvent>()
      .connect<&on_update_tileset_viewport_limits>();
}

}  // namespace tactile
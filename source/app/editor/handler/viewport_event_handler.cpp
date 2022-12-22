/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "viewport_event_handler.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/tile/tileset_bundle.hpp"
#include "core/type/math.hpp"
#include "core/viewport.hpp"
#include "editor/app_context.hpp"
#include "model/document/map_document.hpp"
#include "model/event/viewport_events.hpp"
#include "model/model.hpp"
#include "ui/conversions.hpp"
#include "ui/viewport/map_viewport.hpp"

namespace tactile {
namespace {

inline constexpr Float2 viewport_default_tile_size = {64, 64};

void on_pan_left()
{
  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_left();
  }
}

void on_pan_right()
{
  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_right();
  }
}

void on_pan_up()
{
  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_up();
  }
}

void on_pan_down()
{
  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_down();
  }
}

void on_reset_zoom()
{
  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.set_tile_size(viewport_default_tile_size);
  }
}

void on_increase_zoom()
{
  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.zoom_in(ui::to_vec(ImGui::GetIO().MousePos));
  }
}

void on_decrease_zoom()
{
  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.zoom_out(ui::to_vec(ImGui::GetIO().MousePos));
  }
}

void on_center_viewport()
{
  ui::center_map_viewport();
}

void on_offset_document_viewport(const OffsetDocumentViewportEvent& event)
{
  // FIXME possible crash if tileset is removed when offsetting viewport
  auto& model = get_model();
  if (auto* document = model.active_document()) {
    auto& viewport = document->get_viewport();
    viewport.offset(event.delta);
  }
}

void on_offset_tileset_viewport(const OffsetTilesetViewportEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tileset_ref = document->get_map().tileset_bundle().get_ref(event.tileset_id);
    auto& viewport = tileset_ref.get_viewport();
    viewport.offset(event.delta);
  }
}

void on_update_tileset_viewport_limits(const UpdateTilesetViewportLimitsEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tileset_ref = document->get_map().tileset_bundle().get_ref(event.tileset_id);
    auto& viewport = tileset_ref.get_viewport();
    viewport.set_limits({event.min_offset, event.max_offset});
  }
}

}  // namespace

void install_viewport_event_handler()
{
  auto& dispatcher = get_dispatcher();

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
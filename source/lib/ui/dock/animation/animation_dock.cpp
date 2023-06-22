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

#include "animation_dock.hpp"

#include <algorithm>  // count_if

#include <imgui.h>

#include "common/type/maybe.hpp"
#include "common/util/lookup.hpp"
#include "io/proto/settings.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/setting_events.hpp"
#include "model/events/tileset_events.hpp"
#include "model/model.hpp"
#include "model/systems/language_system.hpp"
#include "model/textures/texture_components.hpp"
#include "model/textures/texture_system.hpp"
#include "model/tiles/tile_components.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/tilesets/tileset_system.hpp"
#include "ui/constants.hpp"
#include "ui/conversions.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

inline constexpr ImVec2 kFrameImageSize {32, 32};
inline constexpr int64 kMinFrameDurationMs = 1;
inline constexpr int64 kMaxFrameDurationMs = 100'000;

void _push_animated_tile_cell_image(const Model& model,
                                    const Tileset& tileset,
                                    const TileIndex tile_index,
                                    const ImVec2& cell_image_size,
                                    const ImVec4& tint = ImVec4 {1, 1, 1, 1})
{
  const auto& texture = model.get<Texture>(tileset.texture);

  const Float2 tile_size = tileset.tile_size;
  const Float2 texture_size = texture.size;

  const auto tile_pos = TilePos::from_index(tile_index, tileset.column_count);

  const auto uv_min = (tile_pos.as_vec2f() * tile_size) / texture_size;
  const auto uv_max = uv_min + tileset.uv_size;

  ImGui::Image(texture.handle,
               cell_image_size,
               as_imvec2(uv_min),
               as_imvec2(uv_max),
               tint);
}

[[nodiscard]] auto _push_frame_duration_slider(const Strings& strings,
                                               const ms_t duration) -> Maybe<ms_t>
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(strings.animation_dock.duration.c_str());
  ImGui::SameLine();

  int64 duration_raw = duration.count();
  if (ImGui::SliderScalar("##Duration",
                          ImGuiDataType_S64,
                          &duration_raw,
                          &kMinFrameDurationMs,
                          &kMaxFrameDurationMs,
                          "%lli ms",
                          ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_AlwaysClamp)) {
    return ms_t {duration_raw};
  }
  else {
    return nothing;
  }
}

void _push_frame_popup(const Model& model,
                       const Strings& strings,
                       const Entity tile_entity,
                       const usize frame_index,
                       const ssize parent_tile_frame_count,
                       Dispatcher& dispatcher)
{
  const auto document_entity = sys::get_active_document(model);

  const auto& tile = model.get<Tile>(tile_entity);
  const auto& tile_animation = model.get<TileAnimation>(tile_entity);
  const auto& frame = tile_animation.frames.at(frame_index);

  if (const Popup popup {"##FramePopup"}; popup.is_open()) {
    if (auto new_frame_duration = _push_frame_duration_slider(strings, frame.duration)) {
      dispatcher.enqueue<SetAnimationFrameDurationEvent>(tile_entity,
                                                         frame_index,
                                                         *new_frame_duration);
    }

    ImGui::Separator();

    if (const Disable disable_if {frame_index == 0};
        ImGui::MenuItem(strings.animation_dock.move_frame_forwards.c_str())) {
      dispatcher.enqueue<MoveAnimationFrameForwardsEvent>(tile_entity, frame_index);
    }

    if (const Disable disable_if {frame_index == tile_animation.frames.size() - 1};
        ImGui::MenuItem(strings.animation_dock.move_frame_backwards.c_str())) {
      dispatcher.enqueue<MoveAnimationFrameBackwardsEvent>(tile_entity, frame_index);
    }

    ImGui::Separator();

    if (const Disable disable_if {tile.index == frame.tile_index};
        ImGui::MenuItem(strings.animation_dock.select_referenced_tile.c_str())) {
      dispatcher.enqueue<SelectTilesetTileEvent>(document_entity, frame.tile_index);
    }

    ImGui::Separator();

    if (const Disable disable_if {tile.index == frame.tile_index &&
                                  parent_tile_frame_count < 2};
        ImGui::MenuItem(strings.animation_dock.remove_frame.c_str())) {
      dispatcher.enqueue<RemoveAnimationFrameEvent>(tile_entity, frame_index);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.animation_dock.delete_animation.c_str())) {
      dispatcher.enqueue<DeleteTileAnimationEvent>(tile_entity);
    }
  }
}

void _push_animation_frame_list(const Model& model,
                                const Strings& strings,
                                const Tileset& tileset,
                                const Entity tile_entity,
                                Dispatcher& dispatcher)
{
  const auto& style = ImGui::GetStyle();

  const ImVec2 child_size {-kMinFloat,
                           kFrameImageSize.y + style.FramePadding.y * 2 +
                               style.ItemInnerSpacing.y + style.ScrollbarSize};

  if (push_button(TAC_ICON_ADD, nullptr, true, 32, child_size.y)) {
    dispatcher.enqueue<EnableAnimationFrameSelectionMode>();
  }

  ImGui::SameLine();

  if (const Child list_child {"##ListChild",
                              child_size,
                              true,
                              ImGuiWindowFlags_AlwaysHorizontalScrollbar};
      list_child.is_open()) {
    if (model.has<TileAnimation>(tile_entity)) {
      const auto& tile = model.get<Tile>(tile_entity);
      const auto& animation = model.get<TileAnimation>(tile_entity);

      // Figure out how many frames in the animation reference the parent (animated) tile.
      const ssize parent_tile_frame_count =
          std::ranges::count_if(animation.frames, [&](const TileAnimationFrame& frame) {
            return tile.index == frame.tile_index;
          });

      usize frame_index = 0;
      for (const auto& frame: animation.frames) {
        const Scope frame_scope {&frame};

        const auto frame_tint = (frame_index == animation.index)
                                    ? ImVec4 {1, 1, 1, 1}
                                    : ImVec4 {0.5, 0.5f, 0.5f, 1};

        ImGui::SameLine();
        _push_animated_tile_cell_image(model,
                                       tileset,
                                       frame.tile_index,
                                       kFrameImageSize,
                                       frame_tint);

        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
          ImGui::OpenPopup("##FramePopup");
        }

        _push_frame_popup(model,
                          strings,
                          tile_entity,
                          frame_index,
                          parent_tile_frame_count,
                          dispatcher);

        ++frame_index;
      }
    }
    else {
      push_centered_label(strings.animation_dock.tile_has_no_animation.c_str());
    }
  }
}

void _push_tile_animation_preview_section(const Model& model,
                                          const Strings& strings,
                                          AnimationDockState& state,
                                          const Tileset& tileset,
                                          const TileIndex tile_index)
{
  ImGui::VSliderFloat("##Size",
                      {32, ImGui::GetContentRegionAvail().y},
                      &state.preview_animation_size,
                      0.1f,
                      1.0f,
                      "");
  push_lazy_tooltip("##SizeTooltip", strings.misc.size.c_str());

  ImGui::SameLine();

  if (const Child child {"##PreviewChild", {0, 0}, true}; child.is_open()) {
    push_centered_label(strings.misc.preview.c_str());

    const auto& texture = model.get<Texture>(tileset.texture);
    const Float2 texture_size = texture.size;

    const auto image_width = ImGui::GetWindowSize().x * state.preview_animation_size;
    const auto image_height = image_width * (texture_size.y / texture_size.x);
    const ImVec2 cell_image_size {image_width, image_height};

    center_next_item_horizontally(image_width);
    _push_animated_tile_cell_image(model, tileset, tile_index, cell_image_size);
  }
}

}  // namespace

void push_animation_dock_widget(const Model& model,
                                AnimationDockState& state,
                                Dispatcher& dispatcher)
{
  const auto& settings = model.get<Settings>();

  if (!settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT)) {
    return;
  }

  const auto& strings = sys::get_current_language_strings(model);

  const auto document_entity = sys::get_active_document(model);
  const auto& tileset_document = model.get<TilesetDocument>(document_entity);
  const auto& tileset = model.get<Tileset>(tileset_document.tileset);

  bool show_animation_dock = true;
  const Window dock {strings.window.animation_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse,
                     &show_animation_dock};

  if (show_animation_dock != settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT)) {
    dispatcher.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_ANIMATION_DOCK_BIT,
                                            show_animation_dock);
  }

  if (dock.is_open()) {
    if (tileset.selected_tile_index.has_value()) {
      const auto selected_tile_entity =
          lookup_in(tileset.tile_index_map, *tileset.selected_tile_index);

      const auto appearance_tile_index =
          sys::get_tile_appearance(model,
                                   tileset_document.tileset,
                                   *tileset.selected_tile_index);

      _push_animation_frame_list(model,
                                 strings,
                                 tileset,
                                 selected_tile_entity,
                                 dispatcher);
      _push_tile_animation_preview_section(model,
                                           strings,
                                           state,
                                           tileset,
                                           appearance_tile_index);
    }
    else {
      prepare_vertical_alignment_center();
      push_centered_label(strings.animation_dock.no_selected_tile_hint.c_str());
    }
  }
}

auto is_animation_dock_enabled(const Model& model) -> bool
{
  return sys::is_tileset_document_active(model);
}

}  // namespace tactile::ui

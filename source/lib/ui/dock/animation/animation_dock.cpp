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
#include <chrono>     // milliseconds

#include <imgui.h>

#include "core/ecs/registry.hpp"
#include "io/settings_io.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/setting_events.hpp"
#include "model/events/tileset_events.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "model/textures/texture_components.hpp"
#include "model/tiles/tile_components.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/tilesets/tileset_system.hpp"
#include "tactile/core/containers/lookup.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "ui/constants.hpp"
#include "ui/conversions.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {
namespace {

inline constexpr ImVec2 kFrameImageSize {32, 32};
inline constexpr int64 kMinFrameDurationMs = 1;
inline constexpr int64 kMaxFrameDurationMs = 100'000;

void _push_animated_tile_cell_image(const Registry& registry,
                                    const Tileset& tileset,
                                    const TileIndex tile_index,
                                    const ImVec2& cell_image_size,
                                    const ImVec4& tint = ImVec4 {1, 1, 1, 1})
{
  const auto& texture = registry.get<Texture>(tileset.texture);

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
                                               const std::chrono::milliseconds duration)
    -> Maybe<std::chrono::milliseconds>
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
    return std::chrono::milliseconds {duration_raw};
  }
  else {
    return kNone;
  }
}

void _push_frame_popup(ModelView& model,
                       const Entity tile_entity,
                       const usize frame_index,
                       const ssize parent_tile_frame_count)
{
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();
  const auto document_entity = sys::get_active_document(registry);

  const auto& tile = registry.get<Tile>(tile_entity);
  const auto& tile_animation = registry.get<TileAnimation>(tile_entity);
  const auto& frame = tile_animation.frames.at(frame_index);

  if (const ui::Popup popup {"##FramePopup"}; popup.is_open()) {
    if (auto new_frame_duration = _push_frame_duration_slider(strings, frame.duration)) {
      model.enqueue<SetAnimationFrameDurationEvent>(tile_entity,
                                                    frame_index,
                                                    *new_frame_duration);
    }

    ImGui::Separator();

    if (const ui::Disable disable_if {frame_index == 0};
        ImGui::MenuItem(strings.animation_dock.move_frame_forwards.c_str())) {
      model.enqueue<MoveAnimationFrameForwardsEvent>(tile_entity, frame_index);
    }

    if (const ui::Disable disable_if {frame_index == tile_animation.frames.size() - 1};
        ImGui::MenuItem(strings.animation_dock.move_frame_backwards.c_str())) {
      model.enqueue<MoveAnimationFrameBackwardsEvent>(tile_entity, frame_index);
    }

    ImGui::Separator();

    if (const ui::Disable disable_if {tile.index == frame.tile_index};
        ImGui::MenuItem(strings.animation_dock.select_referenced_tile.c_str())) {
      model.enqueue<SelectTilesetTileEvent>(document_entity, frame.tile_index);
    }

    ImGui::Separator();

    if (const ui::Disable disable_if {tile.index == frame.tile_index &&
                                      parent_tile_frame_count < 2};
        ImGui::MenuItem(strings.animation_dock.remove_frame.c_str())) {
      model.enqueue<RemoveAnimationFrameEvent>(tile_entity, frame_index);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.animation_dock.delete_animation.c_str())) {
      model.enqueue<DeleteTileAnimationEvent>(tile_entity);
    }
  }
}

void _push_animation_frame_list(ModelView& model,
                                const Tileset& tileset,
                                const Entity tile_entity)
{
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();

  const auto& style = ImGui::GetStyle();

  const ImVec2 child_size {-kMinFloat,
                           kFrameImageSize.y + style.FramePadding.y * 2 +
                               style.ItemInnerSpacing.y + style.ScrollbarSize};

  if (ui::push_button(TAC_ICON_ADD, nullptr, true, 32, child_size.y)) {
    model.enqueue<EnableAnimationFrameSelectionMode>();
  }

  ImGui::SameLine();

  if (const ui::Child list_child {"##ListChild",
                                  child_size,
                                  true,
                                  ImGuiWindowFlags_AlwaysHorizontalScrollbar};
      list_child.is_open()) {
    if (registry.has<TileAnimation>(tile_entity)) {
      const auto& tile = registry.get<Tile>(tile_entity);
      const auto& animation = registry.get<TileAnimation>(tile_entity);

      // Figure out how many frames in the animation reference the parent (animated) tile.
      const ssize parent_tile_frame_count =
          std::ranges::count_if(animation.frames, [&](const TileAnimationFrame& frame) {
            return tile.index == frame.tile_index;
          });

      usize frame_index = 0;
      for (const auto& frame: animation.frames) {
        const ui::Scope frame_scope {&frame};

        const auto frame_tint = (frame_index == animation.index)
                                    ? ImVec4 {1, 1, 1, 1}
                                    : ImVec4 {0.5, 0.5f, 0.5f, 1};

        ImGui::SameLine();
        _push_animated_tile_cell_image(registry,
                                       tileset,
                                       frame.tile_index,
                                       kFrameImageSize,
                                       frame_tint);

        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
          ImGui::OpenPopup("##FramePopup");
        }

        _push_frame_popup(model, tile_entity, frame_index, parent_tile_frame_count);

        ++frame_index;
      }
    }
    else {
      ui::push_centered_label(strings.animation_dock.tile_has_no_animation.c_str());
    }
  }
}

void _push_tile_animation_preview_section(ModelView& model,
                                          AnimationDockState& state,
                                          const Tileset& tileset,
                                          const TileIndex tile_index)
{
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();

  ImGui::VSliderFloat("##Size",
                      {32, ImGui::GetContentRegionAvail().y},
                      &state.preview_animation_size,
                      0.1f,
                      1.0f,
                      "");
  ui::push_lazy_tooltip("##SizeTooltip", strings.misc.size.c_str());

  ImGui::SameLine();

  if (const ui::Child child {"##PreviewChild", {0, 0}, true}; child.is_open()) {
    ui::push_centered_label(strings.misc.preview.c_str());

    const auto& texture = registry.get<Texture>(tileset.texture);
    const Float2 texture_size = texture.size;

    const auto image_width = ImGui::GetWindowSize().x * state.preview_animation_size;
    const auto image_height = image_width * (texture_size.y / texture_size.x);
    const ImVec2 cell_image_size {image_width, image_height};

    ui::center_next_item_horizontally(image_width);
    _push_animated_tile_cell_image(registry, tileset, tile_index, cell_image_size);
  }
}

}  // namespace

void push_animation_dock_widget(ModelView& model, AnimationDockState& state)
{
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();
  const auto& settings = model.get_settings();

  if (!settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT)) {
    return;
  }

  const auto document_entity = sys::get_active_document(registry);
  const auto& tileset_document = registry.get<TilesetDocument>(document_entity);
  const auto& tileset = registry.get<Tileset>(tileset_document.tileset);

  bool show_animation_dock = true;
  const ui::Window dock {strings.window.animation_dock.c_str(),
                         ImGuiWindowFlags_NoCollapse,
                         &show_animation_dock};

  if (show_animation_dock != settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT)) {
    model.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_ANIMATION_DOCK_BIT,
                                       show_animation_dock);
  }

  if (dock.is_open()) {
    if (tileset.selected_tile_index.has_value()) {
      const auto selected_tile_entity =
          lookup_in(tileset.tile_index_map, *tileset.selected_tile_index);

      const auto appearance_tile_index =
          sys::get_tile_appearance(registry,
                                   tileset_document.tileset,
                                   *tileset.selected_tile_index);

      _push_animation_frame_list(model, tileset, selected_tile_entity);
      _push_tile_animation_preview_section(model, state, tileset, appearance_tile_index);
    }
    else {
      ui::prepare_vertical_alignment_center();
      ui::push_centered_label(strings.animation_dock.no_selected_tile_hint.c_str());
    }
  }
}

auto is_animation_dock_enabled(const Registry& registry) -> bool
{
  return sys::is_tileset_document_active(registry);
}

}  // namespace tactile

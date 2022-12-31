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

#include "animation_dock.hpp"

#include <algorithm>  // count_if

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/tile/tile.hpp"
#include "common/type/maybe.hpp"
#include "core/util/fmt.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tileset_events.hpp"
#include "model/model.hpp"
#include "ui/constants.hpp"
#include "ui/conversions.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/textures.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

inline constexpr ImVec2 frame_image_size {32, 32};
inline constexpr int64 min_frame_duration_ms = 1;
inline constexpr int64 max_frame_duration_ms = 100'000;

inline constinit float dock_preview_animation_size = 0.4f;

void ui_tileset_tile_image(const Tileset& tileset,
                           const TileIndex tile_index,
                           const ImVec2& image_size,
                           const ImVec4& tint = ImVec4 {1, 1, 1, 1})
{
  const auto& texture = tileset.texture();

  const Vec2 tile_size = tileset.tile_size();
  const Vec2 texture_size = texture.get_size();

  const auto tile_pos = TilePos::from_index(tile_index, tileset.column_count());

  const auto uv_min = (tile_pos.as_vec2f() * tile_size) / texture_size;
  const auto uv_max = uv_min + tileset.uv_size();

  ImGui::Image(to_texture_id(texture.get_id()),
               image_size,
               from_vec(uv_min),
               from_vec(uv_max),
               tint);
}

[[nodiscard]] auto ui_frame_duration_slider(const Strings& lang, const ms_t duration)
    -> Maybe<ms_t>
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(lang.animation_dock.duration.c_str());
  ImGui::SameLine();

  int64 duration_raw = duration.count();
  if (ImGui::SliderScalar("##Duration",
                          ImGuiDataType_S64,
                          &duration_raw,
                          &min_frame_duration_ms,
                          &max_frame_duration_ms,
                          "%lli ms",
                          ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_AlwaysClamp)) {
    return ms_t {duration_raw};
  }
  else {
    return nothing;
  }
}

void ui_frame_popup(const Strings& lang,
                    const Tile& tile,
                    const TileAnimation::Frame& frame,
                    const usize frame_index,
                    const ssize parent_tile_frame_count,
                    entt::dispatcher& dispatcher)
{
  if (const Popup popup {"##FramePopup"}; popup.is_open()) {
    if (auto new_duration = ui_frame_duration_slider(lang, frame.duration)) {
      dispatcher.enqueue<SetTileAnimationFrameDurationEvent>(tile.get_index(),
                                                             frame_index,
                                                             *new_duration);
    }

    ImGui::Separator();

    {
      const Disable disable_if_first {frame_index == 0};
      if (ImGui::MenuItem(lang.animation_dock.move_frame_forwards.c_str())) {
        dispatcher.enqueue<MoveAnimationFrameForwardsEvent>(frame_index);
      }
    }

    {
      const auto frame_count = tile.get_animation().size();
      const Disable disable_if_last {frame_index == frame_count - 1};
      if (ImGui::MenuItem(lang.animation_dock.move_frame_backwards.c_str())) {
        dispatcher.enqueue<MoveAnimationFrameBackwardsEvent>(frame_index);
      }
    }

    ImGui::Separator();

    {
      const Disable disable_if_already_selected {tile.get_index() == frame.tile};
      if (ImGui::MenuItem(lang.animation_dock.select_referenced_tile.c_str())) {
        dispatcher.enqueue<SelectTilesetTileEvent>(frame.tile);
      }
    }

    ImGui::Separator();

    {
      const Disable disable_if_parent_frame {tile.get_index() == frame.tile &&
                                             parent_tile_frame_count < 2};
      if (ImGui::MenuItem(lang.animation_dock.remove_frame.c_str())) {
        dispatcher.enqueue<RemoveTileAnimationFrameEvent>(frame_index);
      }
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.animation_dock.delete_animation.c_str())) {
      dispatcher.enqueue<DeleteTileAnimationEvent>();
    }
  }
}

void ui_animation_frame(const Strings& lang,
                        const Tileset& tileset,
                        const Tile& tile,
                        const TileAnimation::Frame& frame,
                        const usize frame_index,
                        const ImVec4& tint,
                        const ssize parent_tile_frame_count,
                        entt::dispatcher& dispatcher)
{
  const Scope frame_scope {&frame};

  ui_tileset_tile_image(tileset, frame.tile, frame_image_size, tint);

  if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
    ImGui::OpenPopup("##FramePopup");
  }

  ui_frame_popup(lang, tile, frame, frame_index, parent_tile_frame_count, dispatcher);
}

void ui_animation_frame_list(const Strings& lang,
                             const Tileset& tileset,
                             const Tile& tile,
                             entt::dispatcher& dispatcher)
{
  const auto& style = ImGui::GetStyle();
  const ImVec2 child_size {-min_float,
                           frame_image_size.y + style.FramePadding.y * 2 +
                               style.ItemInnerSpacing.y + style.ScrollbarSize};

  if (ui_button(TAC_ICON_ADD, nullptr, true, 32, child_size.y)) {
    dispatcher.enqueue<EnableAnimationFrameSelectionMode>();
  }

  ImGui::SameLine();

  if (const Child list_child {"##ListChild",
                              child_size,
                              true,
                              ImGuiWindowFlags_AlwaysHorizontalScrollbar};
      list_child.is_open()) {
    if (tile.is_animated()) {
      const auto& animation = tile.get_animation();
      const auto& current_frame = animation.current_frame();

      // Figure out how many frames in the animation reference the parent (animated) tile.
      const ssize parent_tile_frame_count =
          std::count_if(animation.begin(),
                        animation.end(),
                        [&](const TileAnimation::Frame& frame) {
                          return tile.get_index() == frame.tile;
                        });

      usize frame_index = 0;
      for (const auto& frame: animation) {
        ImGui::SameLine();

        // We compare pointers to avoid highlighting several frames when more than
        // one frame reference the same tile.
        const auto frame_tint = &current_frame == &frame ? ImVec4 {1, 1, 1, 1}  //
                                                         : ImVec4 {0.5, 0.5f, 0.5f, 1};

        ui_animation_frame(lang,
                           tileset,
                           tile,
                           frame,
                           frame_index,
                           frame_tint,
                           parent_tile_frame_count,
                           dispatcher);

        ++frame_index;
      }
    }
    else {
      ui_centered_label(lang.animation_dock.tile_has_no_animation.c_str());
    }
  }
}

void ui_tile_animation_preview_section(const Strings& lang,
                                       const Tileset& tileset,
                                       const TileIndex tile_index)
{
  ImGui::VSliderFloat("##Size",
                      {32, ImGui::GetContentRegionAvail().y},
                      &dock_preview_animation_size,
                      0.1f,
                      1.0f,
                      "");
  ui_lazy_tooltip("##SizeTooltip", lang.misc.size.c_str());

  ImGui::SameLine();

  if (const Child child {"##PreviewChild", {0, 0}, true}; child.is_open()) {
    ui_centered_label(lang.misc.preview.c_str());

    const Vec2 texture_size = tileset.texture().get_size();

    const auto image_width = ImGui::GetWindowSize().x * dock_preview_animation_size;
    const auto image_height = image_width * (texture_size.y / texture_size.x);
    const ImVec2 image_size {image_width, image_height};

    center_next_item_horizontally(image_width);
    ui_tileset_tile_image(tileset, tile_index, image_size);
  }
}

void ui_selected_tile_contents(const Strings& lang,
                               const Tileset& tileset,
                               const TileIndex tile_index,
                               entt::dispatcher& dispatcher)
{
  const auto& tile = tileset[tile_index];
  ui_animation_frame_list(lang, tileset, tile, dispatcher);
  ui_tile_animation_preview_section(lang, tileset, tileset.appearance_of(tile_index));
}

}  // namespace

void ui_animation_dock(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& prefs = io::get_preferences();

  if (!prefs.show_animation_dock) {
    return;
  }

  const auto& lang = get_current_language();
  const auto& document = model.require_active_tileset_document();

  const Window dock {lang.window.animation_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse,
                     &prefs.show_animation_dock};

  if (dock.is_open()) {
    const auto& tileset = document.get_tileset();

    if (const auto selected_tile_index = tileset.get_selected_tile()) {
      ui_selected_tile_contents(lang, tileset, *selected_tile_index, dispatcher);
    }
    else {
      prepare_vertical_alignment_center();
      ui_centered_label(lang.animation_dock.no_selected_tile_hint.c_str());
    }
  }
}

}  // namespace tactile::ui

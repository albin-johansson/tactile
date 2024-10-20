// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "stamp_tool.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>

#include "common/util/functional.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tile_pos.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/numeric/random.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

void StampTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void StampTool::on_disabled(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void StampTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void StampTool::on_pressed(DocumentModel& model,
                           entt::dispatcher&,
                           const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_usable(model)) {
    mPrevious.clear();
    mCurrent.clear();

    mLastChangedPos.reset();

    update_sequence(model, mouse.position_in_viewport);
  }
}

void StampTool::on_dragged(DocumentModel& model,
                           entt::dispatcher&,
                           const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_usable(model)) {
    update_sequence(model, mouse.position_in_viewport);
  }
}

void StampTool::on_released(DocumentModel& model,
                            entt::dispatcher& dispatcher,
                            const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_usable(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

void StampTool::set_random(const bool random)
{
  mRandomMode = random;
}

auto StampTool::is_random() const -> bool
{
  return mRandomMode;
}

auto StampTool::behaves_as_if_random(const Map& map) const -> bool
{
  return mRandomMode && map.is_stamp_randomizer_possible();
}

auto StampTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_map_document();
  return document.get_map().is_active_layer(LayerType::TileLayer);
}

void StampTool::update_sequence(DocumentModel& model, const TilePos& cursor)
{
  TACTILE_ASSERT(is_usable(model));

  auto& map_document = model.require_active_map_document();
  auto& map = map_document.get_map();

  const auto active_layer_id = map.get_active_layer_id().value();
  auto& layer = map.get_invisible_root().get_tile_layer(active_layer_id);

  const auto& tilesets = map.get_tileset_bundle();
  const auto tileset_id = tilesets.get_active_tileset_id().value();
  const auto& tileset_ref = tilesets.get_tileset_ref(tileset_id);

  if (behaves_as_if_random(map)) {
    update_sequence_random(layer, tileset_ref, cursor);
  }
  else {
    update_sequence_normal(layer, tileset_ref, cursor);
  }
}

void StampTool::update_sequence_normal(TileLayer& layer,
                                       const TilesetRef& tileset_ref,
                                       const TilePos& cursor)
{
  const auto& tileset = tileset_ref.get_tileset();
  const auto& selection = tileset_ref.get_selection().value();
  const auto selection_size = selection.end - selection.begin;
  const auto preview_offset = selection_size / TilePos {2, 2};

  invoke_mn(selection_size.row(), selection_size.col(), [&](int32 row, int32 col) {
    const TilePos index {row, col};
    const auto selection_pos = selection.begin + index;

    const auto tile = tileset_ref.get_first_tile() + tileset.index_of(selection_pos);
    if (tile != kEmptyTile) {
      const auto pos = cursor + index - preview_offset;

      if (layer.is_valid(pos)) {
        if (mPrevious.find(pos) == mPrevious.end()) {
          mPrevious.emplace(pos, layer.tile_at(pos).value());
        }
        mCurrent[pos] = tile;
        layer.set_tile(pos, tile);
      }
    }
  });
}

void StampTool::update_sequence_random(TileLayer& layer,
                                       const TilesetRef& tileset_ref,
                                       const TilePos& cursor)
{
  const auto& selection = tileset_ref.get_selection().value();
  const auto selection_size = selection.end - selection.begin;

  if (mLastChangedPos != cursor) {
    const auto index =
        get_random_int(0, (selection_size.row() * selection_size.col()) - 1);
    const auto selection_pos =
        selection.begin + TilePos::from_index(index, selection_size.col());

    const auto& tileset = tileset_ref.get_tileset();
    const auto tile = tileset_ref.get_first_tile() + tileset.index_of(selection_pos);

    if (mPrevious.find(cursor) == mPrevious.end()) {
      mPrevious.emplace(cursor, layer.tile_at(cursor).value());
    }

    mCurrent[cursor] = tile;
    layer.set_tile(cursor, tile);
  }

  mLastChangedPos = cursor;
}

void StampTool::maybe_emit_event(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (!mPrevious.empty() && !mCurrent.empty()) {
    const auto& map_document = model.require_active_map_document();
    const auto& map = map_document.get_map();
    const auto layer_id = map.get_active_layer_id().value();

    dispatcher.enqueue<StampSequenceEvent>(layer_id,
                                           std::move(mPrevious),
                                           std::move(mCurrent));

    mPrevious.clear();
    mCurrent.clear();

    mLastChangedPos.reset();
  }
}

auto StampTool::is_usable(const DocumentModel& model) const -> bool
{
  const auto& map_document = model.require_active_map_document();
  const auto& map = map_document.get_map();
  const auto& tilesets = map.get_tileset_bundle();

  if (const auto tileset_id = tilesets.get_active_tileset_id()) {
    return map.is_active_layer(LayerType::TileLayer) &&
           tilesets.get_tileset_ref(*tileset_id).get_selection().has_value();
  }
  else {
    return false;
  }
}

}  // namespace tactile

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "common/type/tile_cache.hpp"
#include "model/tool/tool.hpp"
#include "tactile/base/container/maybe.hpp"

namespace tactile {

class Map;
class TileLayer;
class TilesetRef;

/// A tool used to draw (or "stamp") selected tileset tiles to tile layers.
///
/// The stamp tool, in its default mode, will simply stamp the selected tiles in the
/// active tileset to tile layers. It also has a "random" mode, where a single tile is
/// randomly picked from the tile selection, and subsequently stamped to the tile layer.
///
/// Changes are applied to the target tile layer immediately, and later submitted
/// as a command when the user ends the sequence.
///
/// \todo Right-click should erase tiles.
class StampTool final : public Tool {
 public:
  void accept(ToolVisitor& visitor) const override;

  void on_disabled(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_exited(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_pressed(DocumentModel& model,
                  entt::dispatcher& dispatcher,
                  const MouseInfo& mouse) override;

  void on_dragged(DocumentModel& model,
                  entt::dispatcher& dispatcher,
                  const MouseInfo& mouse) override;

  void on_released(DocumentModel& model,
                   entt::dispatcher& dispatcher,
                   const MouseInfo& mouse) override;

  void set_random(bool random);

  [[nodiscard]] auto is_random() const -> bool;

  [[nodiscard]] auto behaves_as_if_random(const Map& map) const -> bool;

  [[nodiscard]] auto is_available(const DocumentModel& model) const -> bool override;

  [[nodiscard]] auto get_type() const -> ToolType override { return ToolType::Stamp; }

 private:
  TileCache mPrevious;  ///< Previous tile state.
  TileCache mCurrent;   ///< The current stamp sequence.
  Maybe<TilePos> mLastChangedPos;
  bool mRandomMode {};

  void update_sequence(DocumentModel& model, const TilePos& cursor);

  void update_sequence_normal(TileLayer& layer,
                              const TilesetRef& tileset_ref,
                              const TilePos& cursor);

  void update_sequence_random(TileLayer& layer,
                              const TilesetRef& tileset_ref,
                              const TilePos& cursor);

  void maybe_emit_event(const DocumentModel& model, entt::dispatcher& dispatcher);

  [[nodiscard]] auto is_usable(const DocumentModel& model) const -> bool;
};

}  // namespace tactile

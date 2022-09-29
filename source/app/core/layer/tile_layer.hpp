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

#pragma once

#include "core/common/tile_cache.hpp"
#include "core/common/vocabulary.hpp"
#include "core/layer/layer.hpp"
#include "core/layer/layer_delegate.hpp"
#include "core/type/vector.hpp"

namespace tactile {

class TileLayer final : public Layer {
 public:
  TileLayer();

  TileLayer(usize rows, usize columns);

  ~TileLayer() noexcept override = default;

  [[nodiscard]] static auto make() -> Shared<TileLayer>;

  [[nodiscard]] static auto make(usize rows, usize columns) -> Shared<TileLayer>;

  void accept(ContextVisitor& visitor) const override;
  void accept(LayerVisitor& visitor) override;
  void accept(ConstLayerVisitor& visitor) const override;

  void flood(const TilePos& origin, TileID replacement, Vec<TilePos>* affected = nullptr);

  void add_row();
  void add_column();

  void remove_row();
  void remove_column();

  void resize(usize rows, usize columns);

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  void set_parent(const Maybe<UUID>& parentId) override;

  void set_meta_id(int32 id) override;

  void set_tile(const TilePos& pos, TileID id);

  void set_tiles(const TileCache& cache);

  [[nodiscard]] auto tile_at(const TilePos& pos) const -> TileID;

  [[nodiscard]] auto is_valid(const TilePos& pos) const -> bool;

  [[nodiscard]] auto row_count() const -> usize;
  [[nodiscard]] auto column_count() const -> usize;

  [[nodiscard]] auto get_tiles() const -> const TileMatrix&;

  [[nodiscard]] auto get_opacity() const -> float override;

  [[nodiscard]] auto is_visible() const -> bool override;

  [[nodiscard]] auto clone() const -> Shared<Layer> override;

  [[nodiscard]] auto ctx() -> ContextInfo& override;
  [[nodiscard]] auto ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_parent() const -> Maybe<UUID> override;

  [[nodiscard]] auto get_meta_id() const -> Maybe<int32> override;

  [[nodiscard]] auto get_type() const -> LayerType override
  {
    return LayerType::TileLayer;
  }

 private:
  LayerDelegate mDelegate;
  TileMatrix mTiles;
};

}  // namespace tactile

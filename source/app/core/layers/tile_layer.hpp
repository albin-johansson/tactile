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

#include <vector>  // vector

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/tile_cache.hpp"
#include "core/common/tiles.hpp"
#include "core/fwd.hpp"
#include "core/layers/layer.hpp"
#include "core/layers/layer_delegate.hpp"

namespace tactile::core {

class TileLayer final : public ILayer
{
 public:
  TileLayer();

  TileLayer(usize rows, usize columns);

  ~TileLayer() noexcept override = default;

  [[nodiscard]] static auto make() -> Shared<TileLayer>;

  [[nodiscard]] static auto make(usize rows, usize columns) -> Shared<TileLayer>;

  void accept(ILayerVisitor& visitor) override;
  void accept(IConstLayerVisitor& visitor) const override;

  void flood(const TilePos&        origin,
             TileID                replacement,
             std::vector<TilePos>* affected = nullptr);

  void add_row();
  void add_column();

  void remove_row();
  void remove_column();

  void resize(usize rows, usize columns);

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  void set_parent(const Maybe<UUID>& parentId) override;

  void accept(IContextVisitor& visitor) const override;

  void set_name(std::string name) override;

  void set_tile(const TilePos& pos, TileID id);

  void set_tiles(const TileCache& cache);

  [[nodiscard]] auto tile_at(const TilePos& pos) const -> TileID;

  [[nodiscard]] auto is_valid(const TilePos& pos) const -> bool;

  [[nodiscard]] auto row_count() const -> usize;

  [[nodiscard]] auto column_count() const -> usize;

  [[nodiscard]] auto get_tiles() const -> const TileMatrix&;

  [[nodiscard]] auto get_opacity() const -> float override;

  [[nodiscard]] auto is_visible() const -> bool override;

  [[nodiscard]] auto clone() const -> Shared<ILayer> override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_name() const -> const std::string& override;

  [[nodiscard]] auto get_props() -> PropertyBundle& override;
  [[nodiscard]] auto get_props() const -> const PropertyBundle& override;

  [[nodiscard]] auto get_comps() -> ComponentBundle& override;
  [[nodiscard]] auto get_comps() const -> const ComponentBundle& override;

  [[nodiscard]] auto get_parent() const -> Maybe<UUID> override;

  [[nodiscard]] auto get_type() const -> LayerType override
  {
    return LayerType::TileLayer;
  }

 private:
  LayerDelegate mDelegate;
  TileMatrix    mTiles;
};

}  // namespace tactile::core

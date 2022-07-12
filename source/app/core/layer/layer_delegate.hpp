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

#include <string>  // string

#include "core/common/ints.hpp"
#include "core/common/macros.hpp"
#include "core/common/maybe.hpp"
#include "core/common/uuid.hpp"
#include "core/context/context_delegate.hpp"

namespace tactile {

class LayerDelegate final
{
 public:
  TACTILE_DELETE_COPY(LayerDelegate);
  TACTILE_DEFAULT_MOVE(LayerDelegate);

  LayerDelegate();

  void set_opacity(float opacity);

  void set_visible(bool visible);

  void set_parent(const Maybe<UUID>& id);

  void set_meta_id(int32 id);

  void set_name(std::string name);

  [[nodiscard]] auto get_opacity() const -> float;

  [[nodiscard]] auto is_visible() const -> bool;

  [[nodiscard]] auto get_uuid() const -> const UUID&;

  [[nodiscard]] auto get_parent() const -> const Maybe<UUID>&;

  [[nodiscard]] auto get_meta_id() const -> const Maybe<int32>&;

  [[nodiscard]] auto get_name() const -> const std::string&;

  [[nodiscard]] auto get_props() -> PropertyBundle&;
  [[nodiscard]] auto get_props() const -> const PropertyBundle&;

  [[nodiscard]] auto get_comps() -> ComponentBundle&;
  [[nodiscard]] auto get_comps() const -> const ComponentBundle&;

  [[nodiscard]] auto clone() const -> LayerDelegate;

 private:
  UUID            mId {};
  Maybe<UUID>     mParentId {};
  Maybe<int32>    mMetaId;
  ContextDelegate mContext;
  float           mOpacity {1.0f};
  bool            mVisible : 1 {true};
};

}  // namespace tactile

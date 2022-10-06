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

#include "core/ctx/context_info.hpp"
#include "core/type/maybe.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

class LayerDelegate final {
 public:
  void set_opacity(float opacity);

  void set_visible(bool visible);

  void set_parent(const Maybe<UUID>& id);

  void set_meta_id(int32 id);

  [[nodiscard]] auto get_opacity() const -> float;

  [[nodiscard]] auto is_visible() const -> bool;

  [[nodiscard]] auto get_parent() const -> const Maybe<UUID>&;

  [[nodiscard]] auto get_meta_id() const -> const Maybe<int32>&;

  [[nodiscard]] auto ctx() -> ContextInfo&;
  [[nodiscard]] auto ctx() const -> const ContextInfo&;

  [[nodiscard]] auto clone() const -> LayerDelegate;

 private:
  ContextInfo mContext;
  Maybe<UUID> mParentId {};
  Maybe<int32> mMetaId;
  float mOpacity {1.0f};
  bool mVisible : 1 {true};
};

}  // namespace tactile

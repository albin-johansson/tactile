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

#include "core/common/uuid.hpp"
#include "core/common/vocabulary.hpp"
#include "core/comp/component_bundle.hpp"
#include "core/ctx/property_bundle.hpp"

namespace tactile {

class ContextInfo final {
 public:
  TACTILE_DEFAULT_COPY(ContextInfo);
  TACTILE_DEFAULT_MOVE(ContextInfo);

  ContextInfo() = default;

  explicit ContextInfo(const UUID& id);

  void set_name(std::string name);

  [[nodiscard]] auto clone() const -> ContextInfo;

  [[nodiscard]] auto uuid() const -> const UUID& { return mId; }

  [[nodiscard]] auto name() const -> const std::string& { return mName; }

  [[nodiscard]] auto props() -> PropertyBundle& { return mProps; }
  [[nodiscard]] auto props() const -> const PropertyBundle& { return mProps; }

  [[nodiscard]] auto comps() -> ComponentBundle& { return mComps; }
  [[nodiscard]] auto comps() const -> const ComponentBundle& { return mComps; }

 private:
  UUID mId {make_uuid()};
  std::string mName;
  PropertyBundle mProps;
  ComponentBundle mComps;
};

}  // namespace tactile

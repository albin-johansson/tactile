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

#pragma once

namespace tactile {

/** Provides the top-level render hardware interface (RHI). */
class IRenderDevice {
 public:
  virtual ~IRenderDevice() noexcept = default;

  [[nodiscard]] virtual auto begin_frame() -> bool = 0;

  virtual void end_frame() = 0;

  virtual auto reload_font_resources() -> bool = 0;
};

}  // namespace tactile

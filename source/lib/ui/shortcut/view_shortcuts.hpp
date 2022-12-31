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

#include "shortcut.hpp"

namespace tactile {

struct CenterViewportShortcut final : Shortcut {
  CenterViewportShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct DecreaseViewportZoomShortcut final : Shortcut {
  DecreaseViewportZoomShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct IncreaseViewportZoomShortcut final : Shortcut {
  IncreaseViewportZoomShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct IncreaseFontSizeShortcut final : Shortcut {
  IncreaseFontSizeShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct DecreaseFontSizeShortcut final : Shortcut {
  DecreaseFontSizeShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct PanUpShortcut final : Shortcut {
  PanUpShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct PanDownShortcut final : Shortcut {
  PanDownShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct PanLeftShortcut final : Shortcut {
  PanLeftShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct PanRightShortcut final : Shortcut {
  PanRightShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct ToggleGridShortcut final : Shortcut {
  ToggleGridShortcut();

  void activate(entt::dispatcher& dispatcher) override;
};

struct ToggleLayerHighlightShortcut final : Shortcut {
  ToggleLayerHighlightShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct ToggleUiShortcut final : Shortcut {
  ToggleUiShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

}  // namespace tactile

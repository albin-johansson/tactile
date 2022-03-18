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

struct CenterViewportShortcut final : AShortcut
{
  CenterViewportShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct DecreaseViewportZoomShortcut final : AShortcut
{
  DecreaseViewportZoomShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct IncreaseViewportZoomShortcut final : AShortcut
{
  IncreaseViewportZoomShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct PanUpShortcut final : AShortcut
{
  PanUpShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct PanDownShortcut final : AShortcut
{
  PanDownShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct PanLeftShortcut final : AShortcut
{
  PanLeftShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct PanRightShortcut final : AShortcut
{
  PanRightShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct ToggleGridShortcut final : AShortcut
{
  ToggleGridShortcut();

  void activate(entt::dispatcher& dispatcher) override;
};

struct ToggleLayerHighlightShortcut final : AShortcut
{
  ToggleLayerHighlightShortcut();

  void activate(entt::dispatcher& dispatcher) override;
};

struct ToggleUiShortcut final : AShortcut
{
  ToggleUiShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

}  // namespace tactile

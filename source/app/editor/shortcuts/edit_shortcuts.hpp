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

struct UndoShortcut final : AShortcut
{
  UndoShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct RedoShortcut final : AShortcut
{
  RedoShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct AddRowShortcut final : AShortcut
{
  AddRowShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct AddColumnShortcut final : AShortcut
{
  AddColumnShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct RemoveRowShortcut final : AShortcut
{
  RemoveRowShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct RemoveColumnShortcut final : AShortcut
{
  RemoveColumnShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct AddTilesetShortcut final : AShortcut
{
  AddTilesetShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct EnableBucketShortcut final : AShortcut
{
  EnableBucketShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct EnableEraserShortcut final : AShortcut
{
  EnableEraserShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct EnableStampShortcut final : AShortcut
{
  EnableStampShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct EnableObjectSelectionShortcut final : AShortcut
{
  EnableObjectSelectionShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct EnableRectangleToolShortcut final : AShortcut
{
  EnableRectangleToolShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct EnableEllipseToolShortcut final : AShortcut
{
  EnableEllipseToolShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

struct EnablePointToolShortcut final : AShortcut
{
  EnablePointToolShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model,
                                const WidgetManager& widgets) const -> bool override;
};

}  // namespace tactile
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

#include "shortcut.hpp"

namespace tactile {

struct UndoShortcut final : Shortcut {
  UndoShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct RedoShortcut final : Shortcut {
  RedoShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct AddRowShortcut final : Shortcut {
  AddRowShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct AddColumnShortcut final : Shortcut {
  AddColumnShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct RemoveRowShortcut final : Shortcut {
  RemoveRowShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct RemoveColumnShortcut final : Shortcut {
  RemoveColumnShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct AddTilesetShortcut final : Shortcut {
  AddTilesetShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct EnableBucketShortcut final : Shortcut {
  EnableBucketShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct EnableEraserShortcut final : Shortcut {
  EnableEraserShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct EnableStampShortcut final : Shortcut {
  EnableStampShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct EnableObjectSelectionShortcut final : Shortcut {
  EnableObjectSelectionShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct EnableRectangleToolShortcut final : Shortcut {
  EnableRectangleToolShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct EnableEllipseToolShortcut final : Shortcut {
  EnableEllipseToolShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct EnablePointToolShortcut final : Shortcut {
  EnablePointToolShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

struct OpenSettingsShortcut final : Shortcut {
  OpenSettingsShortcut();

  void activate(entt::dispatcher& dispatcher) override;
};

struct OpenComponentEditorShortcut final : Shortcut {
  OpenComponentEditorShortcut();

  void activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_enabled(const DocumentModel& model) const -> bool override;
};

}  // namespace tactile
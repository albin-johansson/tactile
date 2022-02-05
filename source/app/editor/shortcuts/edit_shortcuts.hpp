#pragma once

#include "shortcut.hpp"

namespace tactile {

struct UndoShortcut final : AShortcut
{
  UndoShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct RedoShortcut final : AShortcut
{
  RedoShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct AddRowShortcut final : AShortcut
{
  AddRowShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct AddColumnShortcut final : AShortcut
{
  AddColumnShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct RemoveRowShortcut final : AShortcut
{
  RemoveRowShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct RemoveColumnShortcut final : AShortcut
{
  RemoveColumnShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct AddTilesetShortcut final : AShortcut
{
  AddTilesetShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct EnableBucketShortcut final : AShortcut
{
  EnableBucketShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct EnableEraserShortcut final : AShortcut
{
  EnableEraserShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct EnableStampShortcut final : AShortcut
{
  EnableStampShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct EnableObjectSelectionShortcut final : AShortcut
{
  EnableObjectSelectionShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

}  // namespace tactile
#pragma once

#include "shortcut.hpp"

namespace tactile {

struct NewMapShortcut final : AShortcut
{
  NewMapShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

struct OpenMapShortcut final : AShortcut
{
  OpenMapShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

struct OpenSettingsShortcut final : AShortcut
{
  OpenSettingsShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

struct SaveShortcut final : AShortcut
{
  SaveShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

struct SaveAsShortcut final : AShortcut
{
  SaveAsShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model, const widget_manager& widgets) const
      -> bool override;
};

}  // namespace tactile

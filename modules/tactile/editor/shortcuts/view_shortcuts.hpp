#pragma once

#include "editor/shortcuts/shortcut.hpp"

namespace Tactile {

struct CenterViewportShortcut final : AShortcut
{
  CenterViewportShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

struct DecreaseViewportZoomShortcut final : AShortcut
{
  DecreaseViewportZoomShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

struct IncreaseViewportZoomShortcut final : AShortcut
{
  IncreaseViewportZoomShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

struct PanUpShortcut final : AShortcut
{
  PanUpShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

struct PanDownShortcut final : AShortcut
{
  PanDownShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

struct PanLeftShortcut final : AShortcut
{
  PanLeftShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

struct PanRightShortcut final : AShortcut
{
  PanRightShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

struct ToggleGridShortcut final : AShortcut
{
  ToggleGridShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

struct ToggleUiShortcut final : AShortcut
{
  ToggleUiShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::ui {

/// \addtogroup UI
/// \{

/**
 * RAII helper for the PushID/PopID ImGui functions.
 */
class IdScope final
{
 public:
  TACTILE_DELETE_COPY(IdScope);
  TACTILE_DELETE_MOVE(IdScope);

  [[nodiscard]]
  explicit IdScope(const void* id);

  [[nodiscard]]
  explicit IdScope(const char* id);

  [[nodiscard]]
  explicit IdScope(int id);

  [[nodiscard]]
  explicit IdScope(EntityID id);

  ~IdScope() noexcept;
};

/**
 * RAII helper for the BeginDisabled/EndDisabled ImGui functions.
 */
class DisabledScope final
{
 public:
  TACTILE_DELETE_COPY(DisabledScope);
  TACTILE_DELETE_MOVE(DisabledScope);

  [[nodiscard]]
  explicit DisabledScope(bool disabled = true);

  ~DisabledScope() noexcept;
};

/**
 * RAII helper for the Indent/Unindent ImGui functions.
 */
class IndentScope final
{
 public:
  TACTILE_DELETE_COPY(IndentScope);
  TACTILE_DELETE_MOVE(IndentScope);

  [[nodiscard]]
  IndentScope();

  ~IndentScope() noexcept;
};

/**
 * RAII helper for the BeginGroup/EndGroup ImGui functions.
 */
class GroupScope final
{
 public:
  TACTILE_DELETE_COPY(GroupScope);
  TACTILE_DELETE_MOVE(GroupScope);

  [[nodiscard]]
  GroupScope();

  ~GroupScope() noexcept;
};

/**
 * RAII helper for the BeginTooltip/EndTooltip ImGui functions.
 */
class TooltipScope final
{
 public:
  TACTILE_DELETE_COPY(TooltipScope);
  TACTILE_DELETE_MOVE(TooltipScope);

  [[nodiscard]]
  TooltipScope();

  ~TooltipScope() noexcept;
};

/// \}

}  // namespace tactile::ui

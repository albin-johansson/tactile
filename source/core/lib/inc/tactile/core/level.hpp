// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/basic/macros.hpp"
#include "tactile/core/basic/smart_ptr.hpp"
#include "tactile/core/fwd.hpp"

namespace tactile {

/// The central data structure that contains all data for a single level.
class TACTILE_CORE_API Level final
{
 public:
  TACTILE_DELETE_COPY(Level);
  TACTILE_DEFAULT_MOVE(Level);

  Level();

  ~Level() noexcept;

  void visit(ILevelVisitor& visitor) const;

  void visit(ILevelRootLayerVisitor& visitor) const;

  [[nodiscard]]
  auto get_data() -> LevelData&;

  [[nodiscard]]
  auto get_data() const -> const LevelData&;

 private:
  Unique<LevelData> m_data;
};

}  // namespace tactile

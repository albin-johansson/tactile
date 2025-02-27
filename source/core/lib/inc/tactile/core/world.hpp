// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/basic/macros.hpp"
#include "tactile/core/basic/smart_ptr.hpp"

namespace tactile {

class WorldData;

/// The central data structure that contains all data for a single level.
class TACTILE_CORE_API World final
{
 public:
  TACTILE_DELETE_COPY(World);
  TACTILE_DEFAULT_MOVE(World);

  World();

  ~World() noexcept;

  [[nodiscard]]
  auto get_data() -> WorldData&;

  [[nodiscard]]
  auto get_data() const -> const WorldData&;

 private:
  Unique<WorldData> m_data;
};

}  // namespace tactile

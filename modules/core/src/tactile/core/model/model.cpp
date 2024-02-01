// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/model/model.hpp"

namespace tactile::core {

auto Model::settings() -> Settings&
{
  return mSettings;
}

auto Model::settings() const -> const Settings&
{
  return mSettings;
}

}  // namespace tactile::core

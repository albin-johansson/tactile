// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/model/model.hpp"

#include <utility>  // move

#include "tactile/foundation/container/lookup.hpp"

namespace tactile::core {

void Model::set_languages(LanguageMap languages)
{
  mLanguages = std::move(languages);
}

auto Model::current_language() const -> const Language&
{
  return lookup_in(mLanguages, mSettings.language);
}

auto Model::settings() -> Settings&
{
  return mSettings;
}

auto Model::settings() const -> const Settings&
{
  return mSettings;
}

}  // namespace tactile::core

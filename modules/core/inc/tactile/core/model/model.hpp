// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/persist/settings.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * \brief Provides the high-level API to the core model.
 */
class TACTILE_CORE_API Model final {
 public:
  void set_languages(LanguageMap languages);

  [[nodiscard]]
  auto current_language() const -> const Language&;

  [[nodiscard]]
  auto settings() -> Settings&;

  [[nodiscard]]
  auto settings() const -> const Settings&;

 private:
  Settings mSettings {};
  LanguageMap mLanguages {};
};

}  // namespace tactile::core

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

#include "language_service.hpp"

#include <utility>  // move

#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/util/lookup.hpp"
#include "io/lang/language_parser.hpp"
#include "model/services/service_locator.hpp"
#include "model/services/settings_service.hpp"

namespace tactile {

void LanguageService::load_languages()
{
  spdlog::debug("[LanguageService] Loading language files");

  auto en = parse_language("assets/lang/en.json");

  mLanguages[Lang::SV] = parse_language("assets/lang/sv.json", en);
  mLanguages[Lang::EN_GB] = parse_language("assets/lang/en_GB.json", en);
  mLanguages[Lang::EN] = std::move(en);
}

auto LanguageService::get_strings(Lang language) const -> const Strings&
{
  return lookup_in(mLanguages, language);
}

auto LanguageService::get_current_language_strings() const -> const Strings&
{
  const auto& settings_service = ServiceLocator<SettingsService>::get();
  const auto& settings = settings_service.current_settings();
  return get_strings(settings.get_language());
}

auto LanguageService::get_name(const Lang language) -> const char*
{
  switch (language) {
    case Lang::EN:
      return "English (US)";

    case Lang::EN_GB:
      return "English (GB)";

    case Lang::SV:
      return "Svenska";

    default:
      throw TactileError {"Unknown language"};
  }
}

}  // namespace tactile

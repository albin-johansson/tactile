/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "language.hpp"

#include <utility>  // move

#include <spdlog/spdlog.h>

#include "core/common/associative.hpp"
#include "lang/strings.hpp"
#include "io/lang/language_parser.hpp"
#include "io/persist/preferences.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

inline HashMap<Lang, Strings> languages;

}  // namespace

void load_languages()
{
  spdlog::info("Loading languages...");

  auto en = io::parse_language("assets/lang/en.json");

  languages[Lang::SV] = io::parse_language("assets/lang/sv.json", en);
  languages[Lang::EN_GB] = io::parse_language("assets/lang/en_GB.json", en);

  languages[Lang::EN] = std::move(en);
}

auto get_language(const Lang lang) -> const Strings&
{
  return lookup_in(languages, lang);
}

auto get_current_language() -> const Strings&
{
  return lookup_in(languages, io::get_preferences().language);
}

auto get_language_name(const Lang lang) -> const char*
{
  switch (lang) {
    case Lang::EN:
      return "English (US)";

    case Lang::EN_GB:
      return "English (GB)";

    case Lang::SV:
      return "Svenska";

    default:
      throw TactileError {"Invalid language!"};
  }
}

}  // namespace tactile

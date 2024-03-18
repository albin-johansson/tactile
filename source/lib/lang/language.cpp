// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "language.hpp"

#include <utility>  // move

#include <spdlog/spdlog.h>

#include "io/lang/language_parser.hpp"
#include "lang/strings.hpp"
#include "model/settings.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {
namespace {

inline HashMap<Lang, Strings> gLanguages;

}  // namespace

void load_languages()
{
  spdlog::debug("Loading languages...");

  auto en = parse_language("assets/lang/en.json");

  gLanguages[Lang::SV] = parse_language("assets/lang/sv.json", en);
  gLanguages[Lang::EN_GB] = parse_language("assets/lang/en_GB.json", en);

  gLanguages[Lang::EN] = std::move(en);
}

auto get_language(const Lang lang) -> const Strings&
{
  return lookup_in(gLanguages, lang);
}

auto get_current_language() -> const Strings&
{
  return lookup_in(gLanguages, get_settings().get_language());
}

auto get_language_name(const Lang lang) -> const char*
{
  switch (lang) {
    case Lang::EN: return "English (US)";

    case Lang::EN_GB: return "English (GB)";

    case Lang::SV: return "Svenska";

    default: throw Exception {"Invalid language!"};
  }
}

}  // namespace tactile

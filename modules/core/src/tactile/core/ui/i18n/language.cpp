// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/i18n/language.hpp"

#include <utility>  // move

#include <magic_enum.hpp>

#include "tactile/core/ui/i18n/icons.hpp"
#include "tactile/core/ui/i18n/language_parser.hpp"
#include "tactile/foundation/container/lookup.hpp"
#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/debug/profiler.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {
namespace {

[[nodiscard]]
auto _validate_strings(Vector<String>& strings, const Language* fallback_language)
    -> Result<void>
{
  usize index {0};

  for (auto& string : strings) {
    if (string.empty()) {
      const StringID string_id {index};

      if (fallback_language) {
        string = fallback_language->get(string_id);
      }
      else {
        TACTILE_LOG_ERROR("String with ID {} ('{}') is missing from translation file",
                          index,
                          magic_enum::enum_name(string_id));
        return unexpected(make_error(GenericError::kInvalidFile));
      }
    }

    ++index;
  }

  return kOK;
}

[[nodiscard]]
auto _load_language_to(LanguageMap& languages,
                       const FilePath& path,
                       const LanguageID id,
                       const Language* fallback = nullptr) -> Result<void>
{
  return Language::parse(path, fallback).transform([&](Language&& language) {
    languages.insert_or_assign(id, std::move(language));
  });
}

}  // namespace

Language::Language(Vector<String> strings)
  : mStrings {std::move(strings)}
{}

auto Language::parse(const FilePath& path, const Language* fallback) -> Result<Language>
{
  const ScopeProfiler profiler {"Language::parse"};

  if (auto strings = parse_language_strings(path)) {
    return _validate_strings(*strings, fallback).transform([&]() {
      return Language {std::move(*strings)};
    });
  }

  return unexpected(make_error(GenericError::kUnknown));
}

void Language::add_icons()
{
  inject_icons(mStrings);
}

auto load_languages(const FilePath& dir) -> Result<LanguageMap>
{
  using enum LanguageID;

  LanguageMap languages {};
  const Language* us_en {};

  return _load_language_to(languages, dir / "en.ini", kAmericanEnglish)
      .transform([&] { us_en = find_in(languages, kAmericanEnglish); })
      .transform([&] {
        for (auto& [language_id, language] : languages) {
          language.add_icons();
        }
      })
      .transform([&] { return std::move(languages); });
}

}  // namespace tactile::core

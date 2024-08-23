// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>       // expected
#include <filesystem>     // path
#include <system_error>   // error_code
#include <unordered_map>  // unordered_map

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/i18n/language_id.hpp"
#include "tactile/core/ui/i18n/string_id.hpp"

namespace tactile::ui {

/**
 * Utility for parsing language INI files.
 */
class LanguageParser final
{
 public:
  TACTILE_DELETE_COPY(LanguageParser);
  TACTILE_DEFAULT_MOVE(LanguageParser);

  LanguageParser();

  ~LanguageParser() noexcept = default;

  /**
   * Attempts to parse a language file.
   *
   * \param id       The language identifier.
   * \param path     The path to the language INI file.
   * \param fallback The fallback language used whenever a string is missing.
   *
   * \return
   * A language if successful; an error code otherwise.
   */
  [[nodiscard]]
  auto parse(LanguageID id,
             const std::filesystem::path& path,
             const Language* fallback = nullptr) const
      -> std::expected<Language, std::error_code>;

 private:
  std::unordered_map<StringView, StringID> mMiscNames {};
  std::unordered_map<StringView, StringID> mVerbNames {};
  std::unordered_map<StringView, StringID> mNounNames {};
  std::unordered_map<StringView, StringID> mAdjectiveNames {};
  std::unordered_map<StringView, StringID> mActionNames {};
  std::unordered_map<StringView, StringID> mHintNames {};
  std::unordered_map<StringView, StringID> mMenuNames {};
  std::unordered_map<StringView, StringID> mWidgetNames {};
};

/**
 * Attempts to parse a language file from disk.
 *
 * \param id The identifier associated with the desired language.
 *
 * \return
 * A parsed language if successful; an error code otherwise.
 */
[[nodiscard]]
auto parse_language_from_disk(LanguageID id) -> std::expected<Language, std::error_code>;

}  // namespace tactile::ui

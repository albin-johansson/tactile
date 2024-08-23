// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>       // expected
#include <filesystem>     // path
#include <string_view>    // string_view
#include <system_error>   // error_code
#include <unordered_map>  // unordered_map

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
  std::unordered_map<std::string_view, StringID> mMiscNames {};
  std::unordered_map<std::string_view, StringID> mVerbNames {};
  std::unordered_map<std::string_view, StringID> mNounNames {};
  std::unordered_map<std::string_view, StringID> mAdjectiveNames {};
  std::unordered_map<std::string_view, StringID> mActionNames {};
  std::unordered_map<std::string_view, StringID> mHintNames {};
  std::unordered_map<std::string_view, StringID> mMenuNames {};
  std::unordered_map<std::string_view, StringID> mWidgetNames {};
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

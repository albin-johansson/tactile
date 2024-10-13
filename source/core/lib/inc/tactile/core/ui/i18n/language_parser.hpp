// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>       // expected
#include <filesystem>     // path
#include <string_view>    // string_view
#include <unordered_map>  // unordered_map

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/i18n/labels.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::core::ui {

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
   * \param id   The language identifier.
   * \param path The path to the language INI file.
   *
   * \return
   * A language if successful; an error code otherwise.
   */
  [[nodiscard]]
  auto parse(LanguageID id, const std::filesystem::path& path) const
      -> std::expected<Language, ErrorCode>;

 private:
  std::unordered_map<std::string_view, MiscLabel> mMiscNames {};
  std::unordered_map<std::string_view, VerbLabel> mVerbNames {};
  std::unordered_map<std::string_view, NounLabel> mNounNames {};
  std::unordered_map<std::string_view, AdjectiveLabel> mAdjectiveNames {};
  std::unordered_map<std::string_view, ActionLabel> mActionNames {};
  std::unordered_map<std::string_view, HintLabel> mHintNames {};
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
auto parse_language_from_disk(LanguageID id) -> std::expected<Language, ErrorCode>;

}  // namespace tactile::core::ui

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t
#include <string>   // string
#include <utility>  // to_underlying
#include <vector>   // vector

#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/ui/i18n/labels.hpp"

namespace tactile::core::ui {

/**
 * Provides identifiers for the supported languages.
 */
enum class LanguageID : std::uint8_t
{
  kAmericanEnglish,
  kBritishEnglish,
  kSwedish,
};

/**
 * Represents a set of translated user-facing strings.
 */
class Language final
{
 public:
  friend class LanguageParser;

  TACTILE_DEFAULT_MOVE(Language);
  TACTILE_DELETE_COPY(Language);

  ~Language() noexcept = default;

  /**
   * Prepends suitable icons to the strings in the language.
   */
  void add_icons();

  /**
   * Returns the identifier associated with the language.
   *
   * \return
   * A language identifier.
   */
  [[nodiscard]]
  auto get_id() const -> LanguageID;

  [[nodiscard]]
  auto get(const MiscLabel id) const noexcept -> const char*
  {
    return _get(std::to_underlying(id));
  }

  [[nodiscard]]
  auto get(const NounLabel id) const noexcept -> const char*
  {
    return _get(std::to_underlying(id));
  }

  [[nodiscard]]
  auto get(const VerbLabel id) const noexcept -> const char*
  {
    return _get(std::to_underlying(id));
  }

  [[nodiscard]]
  auto get(const AdjectiveLabel id) const noexcept -> const char*
  {
    return _get(std::to_underlying(id));
  }

  [[nodiscard]]
  auto get(const ActionLabel id) const noexcept -> const char*
  {
    return _get(std::to_underlying(id));
  }

  [[nodiscard]]
  auto get(const HintLabel id) const noexcept -> const char*
  {
    return _get(std::to_underlying(id));
  }

 private:
  LanguageID mID;
  std::vector<std::string> mStrings {};

  Language(LanguageID id, std::vector<std::string> strings);

  /**
   * Returns the translated string associated with a given identifier.
   *
   * \note
   * This function is called very often, so it's inlined and performs no bounds
   * checks whatsoever to be as fast as possible.
   *
   * \pre The index must be valid.
   *
   * \param index A string index.
   *
   * \return
   * A translated string.
   */
  [[nodiscard]]
  auto _get(const std::size_t index) const noexcept -> const char*
  {
    TACTILE_ASSERT(index < mStrings.size());
    return mStrings[index].c_str();
  }
};

}  // namespace tactile::core::ui

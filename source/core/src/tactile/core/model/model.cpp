// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/model/model.hpp"

#include "tactile/core/debug/validation.hpp"

namespace tactile {

Model::Model(const Settings* settings, const ui::Language* language)
  : mSettings {require_not_null(settings, "null settings")},
    mLanguage {require_not_null(language, "null language")}
{}

auto Model::get_document_manager() -> DocumentManager&
{
  return mDocuments;
}

auto Model::get_document_manager() const -> const DocumentManager&
{
  return mDocuments;
}

auto Model::get_current_document() -> IDocument*
{
  return mDocuments.get_current_document();
}

auto Model::get_current_document() const -> const IDocument*
{
  return mDocuments.get_current_document();
}

auto Model::get_settings() const -> const Settings&
{
  return *mSettings;
}

auto Model::get_language() const -> const ui::Language&
{
  return *mLanguage;
}

}  // namespace tactile

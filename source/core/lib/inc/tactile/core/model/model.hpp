// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // forward

#include "tactile/base/document/document.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/document/document_manager.hpp"
#include "tactile/core/document/map_document.hpp"

namespace tactile {

struct Settings;

namespace ui {
class Language;
}  // namespace ui

/**
 * Provides the top-level API for the core document model.
 *
 * \details
 * Due to the nature of how Dear ImGui works, the model only allows using one
 * language per editor session. This is to avoid issues regarding the handling
 * of translated strings.
 */
class Model final
{
 public:
  /**
   * Creates a model.
   *
   * \param settings The associated settings.
   * \param language The language that will be used throughout this session.
   */
  Model(Settings* settings, const ui::Language* language);

  /**
   * Returns the associated document manager.
   *
   * \return
   * A document manager.
   */
  [[nodiscard]]
  auto get_document_manager() -> DocumentManager&;

  /**
   * \copydoc get_documents()
   */
  [[nodiscard]]
  auto get_document_manager() const -> const DocumentManager&;

  /**
   * Returns the currently active document, if any.
   *
   * \return
   * A pointer to the active document; a null pointer if there is none.
   */
  [[nodiscard]]
  auto get_current_document() -> IDocument*;

  /**
   * \copydoc get_current_document()
   */
  [[nodiscard]]
  auto get_current_document() const -> const IDocument*;

  /**
   * Returns the associated settings.
   *
   * \return
   * The current settings.
   */
  [[nodiscard]]
  auto get_settings() -> Settings&;

  /**
   * \copydoc get_settings()
   */
  [[nodiscard]]
  auto get_settings() const -> const Settings&;

  /**
   * Returns the language used in this session.
   *
   * \return
   * A language.
   */
  [[nodiscard]]
  auto get_language() const -> const ui::Language&;

  /**
   * Attempts to push a command to the command stack of the active document.
   *
   * \tparam T    The command type.
   * \tparam Args The types of the command constructor arguments.
   *
   * \param args The arguments forwarded to a command constructor. Note, all
   *             command constructor are assumed to feature at least an initial
   *             \c IDocument* parameter, so this function automatically
   *             provides it.
   */
  template <typename T, typename... Args>
  void push_command(Args&&... args)
  {
    if (auto* document = get_current_document()) {
      auto& history = mDocuments.get_history(document->get_uuid());
      history.push<T>(document, std::forward<Args>(args)...);
    }
  }

  /**
   * Attempts to push a map command to the command stack of the active document.
   *
   * \tparam T    The command type.
   * \tparam Args The types of the command constructor arguments.
   *
   * \param args The arguments forwarded to a command constructor. Note, all
   *             map command constructor are assumed to feature at least an
   *             initial \c MapDocument* parameter, so this function
   *             automatically provides it.
   */
  template <typename T, typename... Args>
  void push_map_command(Args&&... args)
  {
    if (auto* document = dynamic_cast<MapDocument*>(get_current_document())) {
      auto& history = mDocuments.get_history(document->get_uuid());
      history.push<T>(document, std::forward<Args>(args)...);
    }
  }

 private:
  Settings* mSettings {};
  const ui::Language* mLanguage {};
  DocumentManager mDocuments {};
};

}  // namespace tactile

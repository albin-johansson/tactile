#pragma once

#include <QFileInfo>
#include <QImage>
#include <QObject>
#include <QString>

#include "map_document.hpp"
#include "map_id.hpp"
#include "maybe.hpp"
#include "vector_map.hpp"

namespace tactile::core {

/**
 * \class map_manager
 *
 * \brief Manages multiple map documents.
 *
 * \since 0.1.0
 *
 * \headerfile map_manager.hpp
 */
class map_manager final : public QObject
{
  Q_OBJECT

  using storage_type = vector_map<map_id, map_document*>;

 public:
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;

  explicit map_manager(QObject* parent = nullptr);

  /**
   * \brief Adds a new map document.
   *
   * \return the map ID that associated with the document.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto add() -> map_id;

  /**
   * \brief Adds a new map document.
   *
   * \note The model claims ownership of the supplied document.
   *
   * \param document a pointer to the document that will be added, can't be
   * null.
   *
   * \return the map ID that was associated with the document.
   *
   * \since 0.1.0
   *
   * \signal `undo_state_updated`, `redo_state_updated`, `undo_text_updated`,
   * `redo_text_updated`.
   */
  [[nodiscard]] auto add(map_document* document) -> map_id;

  /**
   * \brief Closes the map document associated with the specified map ID.
   *
   * \pre `id` must be associated with an existing map document.
   *
   * \param id the ID of the map that will be closed.
   *
   * \since 0.1.0
   *
   * \signal `undo_state_updated`, `redo_state_updated`, `undo_text_updated`,
   * `redo_text_updated`.
   */
  void close(map_id id);

  /**
   * \brief Makes the map document associated with the specified ID active.
   *
   * \pre `id` must be associated with an existing map document.
   *
   * \param id the ID of the map that will be selected.
   *
   * \since 0.1.0
   *
   * \signal `undo_state_updated`, `redo_state_updated`, `undo_text_updated`,
   * `redo_text_updated`, `clean_changed`.
   */
  void select(map_id id);

  /**
   * \brief Indicates whether or not there is an active map document.
   *
   * \return `true` if there is an active map document; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto has_active_map() const noexcept -> bool;

  /**
   * \brief Returns the map document associated with the specified map ID.
   *
   * \param id the ID of the desired map document.
   *
   * \return a pointer to the associated map document; `nullptr` if no such
   * document exists.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto at(map_id id) -> map_document*;

  /**
   * \copydoc at()
   */
  [[nodiscard]] auto at(map_id id) const -> const map_document*;

  /**
   * \brief Returns the ID of the currently active map document.
   *
   * \return the ID associated with the current map document; `std::nullopt` if
   * there is no active document.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto current_map_id() const -> maybe<map_id>;

  /**
   * \brief Returns the currently active map document.
   *
   * \return a pointer to the currently active map document; `nullptr` if there
   * is no such document.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto current_document() -> map_document*;

  /**
   * \copydoc current_document()
   */
  [[nodiscard]] auto current_document() const -> const map_document*;

  /**
   * \brief Returns the current tileset in the the current map document.
   *
   * \return the current tileset in the current map document; `nullptr` if there
   * was no active map document or no active tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto current_tileset() const -> const tileset*;

 signals:
  void undo_state_updated(bool canUndo);
  void redo_state_updated(bool canRedo);
  void undo_text_updated(const QString& text);
  void redo_text_updated(const QString& text);
  void clean_changed(bool clean);

  void added_tileset(tileset_id id);
  void removed_tileset(tileset_id id);

  void added_layer(layer_id id, const tile_layer& layer);
  void added_duplicated_layer(layer_id id, const tile_layer& layer);
  void selected_layer(layer_id id, const tile_layer& layer);
  void removed_layer(layer_id id);
  void moved_layer_back(layer_id id);
  void moved_layer_forward(layer_id id);

 private:
  maybe<map_id> m_current;
  storage_type m_documents;
  map_id m_nextId{1};

  void emit_undo_redo_update();
};

}  // namespace tactile::core

#pragma once

#include <QImage>
#include <QObject>
#include <QString>
#include <optional>  // optional

#include "map_document.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::core {

class map_manager final : public QObject
{
  Q_OBJECT

  using storage_type = vector_map<map_id, map_document*>;

 public:
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;

  [[nodiscard]] auto add() -> map_id;

  void close(map_id id);

  void select(map_id id);

  void select_tileset(tileset_id id);

  void update_tileset_selection(position topLeft, position bottomRight);

  [[nodiscard]] auto has_active_map() const noexcept -> bool;

  [[nodiscard]] auto at(map_id id) -> map_document*;

  [[nodiscard]] auto at(map_id id) const -> const map_document*;

  [[nodiscard]] auto current_map() const -> std::optional<map_id>;

  [[nodiscard]] auto current_document() -> map_document*;

  [[nodiscard]] auto current_document() const -> const map_document*;

  [[nodiscard]] auto current_tileset() const -> const tileset*;

  [[nodiscard]] auto begin() noexcept -> iterator
  {
    return m_mapDocuments.begin();
  }

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_mapDocuments.begin();
  }

  [[nodiscard]] auto end() noexcept -> iterator
  {
    return m_mapDocuments.end();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_mapDocuments.end();
  }

 signals:
  void undo_state_updated(bool canUndo);

  void redo_state_updated(bool canRedo);

  void undo_text_updated(const QString& text);

  void redo_text_updated(const QString& text);

  void added_tileset(tileset_id id);

  void removed_tileset(tileset_id id);

 public slots:
  void ui_added_tileset(const QImage& image,
                        const QString& path,
                        const QString& name,
                        tile_width tileWidth,
                        tile_height tileHeight);

  void ui_removed_tileset(tileset_id id);

 private:
  std::optional<map_id> m_currentMapID;
  storage_type m_mapDocuments;
  map_id m_nextMapID{1};

  void emit_undo_redo_update();
};

}  // namespace tactile::core

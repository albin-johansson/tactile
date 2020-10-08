#pragma once

#include <qimage.h>
#include <qobject.h>
#include <qstring.h>

#include <optional>  // optional

#include "map_document.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::core {

class map_manager final : public QObject
{
  Q_OBJECT

 public:
  [[nodiscard]] auto add() -> map_id;

  void close(map_id id);

  void select(map_id id);

  [[nodiscard]] auto add_tileset(const QImage& image,
                                 const QString& path,
                                 const QString& name,
                                 tile_width tileWidth,
                                 tile_height tileHeight)
      -> std::optional<tileset_id>;

  void remove_tileset(tileset_id id);

  void select_tileset(tileset_id id);

  void update_tileset_selection(position topLeft, position bottomRight);

  [[nodiscard]] auto has_active_map() const noexcept -> bool;

  [[nodiscard]] auto at(map_id id) -> map*;

  [[nodiscard]] auto at(map_id id) const -> const map*;

  [[nodiscard]] auto get_document(map_id id) -> map_document*;

  [[nodiscard]] auto get_document(map_id id) const -> const map_document*;

  [[nodiscard]] auto current_document() -> map_document*;

  [[nodiscard]] auto current_document() const -> const map_document*;

  [[nodiscard]] auto current_map() -> map*;

  [[nodiscard]] auto current_map() const -> const map*;

  [[nodiscard]] auto current_tileset() const -> const tileset*;

  [[nodiscard]] auto get_tileset_manager() -> tileset_manager*;

  [[nodiscard]] auto get_tileset_manager() const -> const tileset_manager*;

  [[nodiscard]] auto begin() noexcept
  {
    return m_mapDocuments.begin();
  }

  [[nodiscard]] auto begin() const noexcept
  {
    return m_mapDocuments.begin();
  }

  [[nodiscard]] auto end() noexcept
  {
    return m_mapDocuments.end();
  }

  [[nodiscard]] auto end() const noexcept
  {
    return m_mapDocuments.end();
  }

 signals:
  void switched_map(map_id id);

  void undo_state_updated(bool canUndo);

  void redo_state_updated(bool canRedo);

  void undo_text_updated(const QString& text);

  void redo_text_updated(const QString& text);

 private:
  std::optional<map_id> m_currentMapID;
  vector_map<map_id, map_document*> m_mapDocuments;
  map_id m_nextMapID{1};

  void emit_undo_redo_update();
};

}  // namespace tactile::core

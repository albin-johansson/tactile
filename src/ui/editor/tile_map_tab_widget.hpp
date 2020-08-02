#pragma once
#include <QTabWidget>
#include <unordered_map>

#include "maybe.hpp"

namespace tactile {

class EditorTab;

class TileMapTabWidget final : public QTabWidget {
  Q_OBJECT

 public:
  explicit TileMapTabWidget(QWidget* parent = nullptr);

  ~TileMapTabWidget() noexcept override;

  [[nodiscard]] int add_tile_map_tab(const QString& title) noexcept;

  void remove_tile_map_tab(int id) noexcept;

  void center_viewport(int mapWidth, int mapHeight) noexcept;

  void move_viewport(int dx, int dy) noexcept;

  [[nodiscard]] Maybe<int> active_tab_id() const noexcept;

  [[nodiscard]] Maybe<int> tab_id(int index) const noexcept;

 signals:
  void s_redraw(QPainter& painter);

  void s_remove_tab(int id);

 private:
  [[nodiscard]] EditorTab* get_pane(int index) const noexcept;
};

}  // namespace tactile

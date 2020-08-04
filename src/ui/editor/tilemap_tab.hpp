#pragma once

#include <QTabWidget>
#include <optional>
#include <unordered_map>

#include "tactile_fwd.hpp"

namespace tactile::ui {

class tilemap_tab final : public QTabWidget {
  Q_OBJECT

 public:
  explicit tilemap_tab(QWidget* parent = nullptr);

  ~tilemap_tab() noexcept override;

  [[nodiscard]] auto add_tile_map_tab(const QString& title) noexcept -> int;

  void remove_tile_map_tab(int id) noexcept;

  void center_viewport(int mapWidth, int mapHeight) noexcept;

  void move_viewport(int dx, int dy) noexcept;

  [[nodiscard]] auto active_tab_id() const noexcept -> std::optional<int>;

  [[nodiscard]] auto tab_id(int index) const noexcept -> std::optional<int>;

 signals:
  void request_redraw(QPainter& painter);

  void request_remove_tab(int id);

 private:
  [[nodiscard]] auto get_pane(int index) const noexcept -> tilemap_view*;

 private slots:
  void handle_tab_close(int index);
};

}  // namespace tactile::ui

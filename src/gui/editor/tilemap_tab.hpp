#pragma once

#include <QTabWidget>
#include <optional>
#include <unordered_map>

#include "tactile_fwd.hpp"

namespace tactile::gui {

class tilemap_tab final : public QTabWidget
{
  Q_OBJECT

 public:
  explicit tilemap_tab(QWidget* parent = nullptr);

  ~tilemap_tab() noexcept override;

  void add_tile_map_tab(model::core_model* core,
                        const QString& title,
                        int id) noexcept;

  void remove_tile_map_tab(int id) noexcept;

  void select_tab(int id);

  void center_viewport(int mapWidth, int mapHeight) noexcept;

  void move_viewport(int dx, int dy) noexcept;

  [[nodiscard]] auto active_tab_id() const noexcept -> std::optional<int>;

  [[nodiscard]] auto tab_id(int index) const noexcept -> std::optional<int>;

 signals:
  void request_redraw(QPainter& painter, const QRectF& exposed);

  void request_remove_tab(int id);

 public slots:
  void theme_changed();

 private:
  [[nodiscard]] auto get_pane(int index) const noexcept -> tilemap_view*;

 private slots:
  void handle_tab_close(int index);
};

}  // namespace tactile::gui

#pragma once

#include <QTabWidget>
#include <optional>  // optional

#include "tactile_fwd.hpp"
#include "tactile_types.hpp"

namespace tactile::gui {

class map_tab_widget final : public QTabWidget
{
  Q_OBJECT

 public:
  explicit map_tab_widget(QWidget* parent = nullptr);

  ~map_tab_widget() noexcept override;

  void add_map_tab(not_null<model::tilemap*> map,
                   const QString& title,
                   map_id id) noexcept;

  void remove_map_tab(map_id id) noexcept;

  void select_tab(map_id id);

  void center_viewport(int mapWidth, int mapHeight) noexcept;

  void move_viewport(int dx, int dy) noexcept;

  [[nodiscard]] auto active_tab_id() const noexcept -> std::optional<map_id>;

  [[nodiscard]] auto tab_id(int index) const noexcept -> std::optional<map_id>;

 signals:
  void request_remove_tab(map_id id);

 public slots:
  void theme_changed();

  void redraw();

 private:
  [[nodiscard]] auto get_view(int index) noexcept -> map_view*;
  [[nodiscard]] auto get_view(int index) const noexcept -> const map_view*;

  [[nodiscard]] auto view_for_id(map_id id) noexcept -> map_view*;

 private slots:
  void handle_tab_close(int index);
};

}  // namespace tactile::gui

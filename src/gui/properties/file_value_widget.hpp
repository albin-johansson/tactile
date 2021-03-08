#pragma once

#include <QFileInfo>  // QFileInfo
#include <QWidget>    // QWidget

#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, file_value_widget)

namespace tactile::gui {

class file_value_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit file_value_widget(QWidget* parent = nullptr);

  ~file_value_widget() noexcept override;

  void enter_active_mode();

  void enter_idle_mode();

  void reset_path();

  void set_path(const QString& path);

  [[nodiscard]] auto current_path() const -> QString;

 signals:
  void spawn_dialog();

 private:
  unique<Ui::file_value_widget> m_ui;
};

}  // namespace tactile::gui

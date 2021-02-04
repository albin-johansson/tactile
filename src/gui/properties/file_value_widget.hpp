#pragma once

#include <QFileInfo>  // QFileInfo

#include "forward_declare.hpp"
#include "property_value_widget.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, file_value_widget)

namespace tactile::gui {

class file_value_widget final : public property_value_widget
{
  Q_OBJECT

 public:
  explicit file_value_widget(QWidget* parent = nullptr);

  ~file_value_widget() noexcept override;

  void enter_active_mode() override;

  void enter_idle_mode() override;

  void reset_path();

  void set_path(const QFileInfo& path);

  [[nodiscard]] auto current_path() const -> QFileInfo;

 signals:
  void spawn_dialog();

 private:
  unique<Ui::file_value_widget> m_ui;
};

}  // namespace tactile::gui

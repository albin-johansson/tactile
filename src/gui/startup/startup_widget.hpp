#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, startup_widget)

namespace tactile::gui {

/**
 * \class startup_widget
 *
 * \brief Represents the widget that is displayed when no map is active.
 *
 * \since 0.1.0
 *
 * \headerfile startup_widget.hpp
 */
class startup_widget final : public QWidget
{
 public:
  explicit startup_widget(QWidget* parent = nullptr);

  ~startup_widget() noexcept override;

 private:
  unique<Ui::startup_widget> m_ui;
};

}  // namespace tactile::gui

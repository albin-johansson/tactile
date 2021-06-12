#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_DECLARE_UI(StartupWidget)

namespace tactile {

/**
 * \class StartupWidget
 *
 * \brief Represents the widget that is displayed when no map is active.
 *
 * \since 0.1.0
 *
 * \headerfile startup_widget.hpp
 */
class StartupWidget final : public QWidget
{
 public:
  explicit StartupWidget(QWidget* parent = nullptr);

  ~StartupWidget() noexcept override;

 private:
  Unique<Ui::StartupWidget> mUi;
};

}  // namespace tactile

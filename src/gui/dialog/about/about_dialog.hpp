#pragma once

#include <QDialog>  // QDialog

#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE_UI(AboutDialog)

namespace tactile {

/**
 * \class AboutDialog
 *
 * \brief Represents the "About Tactile" dialog window that displays
 * information about the application.
 *
 * \see `QDialog`
 *
 * \since 0.1.0
 *
 * \headerfile about_dialog.hpp
 */
class AboutDialog final : public QDialog
{
 public:
  explicit AboutDialog(QWidget* parent = nullptr);

  ~AboutDialog() noexcept override;

  static void Spawn();

 private:
  [[maybe_unused]] unique<Ui::AboutDialog> mUi;
};

}  // namespace tactile

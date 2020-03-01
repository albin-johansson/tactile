#pragma once

#include <QDialog>

namespace Ui {

class AboutUI;

}

namespace tactile {

/**
 * The <code>AboutDialog</code> class represents the "About Tactile" dialog
 * window that displays information about the application.
 *
 * @see QDialog
 * @since 0.1.0
 */
class AboutDialog final : public QDialog {
  Q_OBJECT

 public:
  /**
   * @param parent a pointer to the parent widget, defaults to null.
   * @since 0.1.0
   */
  explicit AboutDialog(QWidget* parent = nullptr);

  ~AboutDialog() noexcept override;

 private:
  Ui::AboutUI* m_ui;
};

}  // namespace tactile

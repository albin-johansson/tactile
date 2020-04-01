#pragma once
#include <QDialog>
#include <QIntValidator>

#include "maybe.h"
#include "smart_pointers.h"

class QLineEdit;

namespace Ui {
class TileSheetDialogUI;
}

namespace tactile {

class TileSheetDialog final : public QDialog {
 public:
  explicit TileSheetDialog(QWidget* parent = nullptr);

  ~TileSheetDialog() noexcept override;

  [[nodiscard]] SharedPtr<QImage> chosen_image() const noexcept
  {
    return m_image;
  }

  [[nodiscard]] Maybe<int> chosen_width() const noexcept { return m_width; }

  [[nodiscard]] Maybe<int> chosen_height() const noexcept { return m_height; }

 private:
  Ui::TileSheetDialogUI* m_ui;
  SharedPtr<QImage> m_image;
  Maybe<int> m_width;
  Maybe<int> m_height;
  UniquePtr<QIntValidator> m_validator;
  QPixmap m_defaultImageIcon;

  void validate_input() noexcept;

  [[nodiscard]] bool is_valid() const noexcept;

  [[nodiscard]] QPixmap load_pixmap(const QString& path) const noexcept;

  /**
   * Returns a pointer to the "OK" button.
   *
   * @return a pointer to the "OK" button.
   * @since 0.1.0
   */
  [[nodiscard]] QPushButton* ok_button() const noexcept;

  /**
   * Indicates whether or not the supplied line edit widget has valid input.
   *
   * @param edit the line edit widget that will be checked.
   * @return true if the line edit widget is valid; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] QValidator::State validate(const QLineEdit& edit) const
      noexcept;
};

}  // namespace tactile

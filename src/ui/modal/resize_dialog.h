#pragma once
#include <QDialog>
#include <QValidator>

#include "maybe.h"

namespace Ui {

class ResizeDialogUI;

}

class QIntValidator;
class QPushButton;
class QLineEdit;

namespace tactile {

/**
 * The <code>ResizeDialog</code> class represents the dialog that is used to
 * select the new size of the current tile map.
 *
 * @since 0.1.0
 */
class ResizeDialog final : public QDialog {
  Q_OBJECT

 public:
  /**
   * @param parent a pointer to the parent widget, defaults to null.
   * @since 0.1.0
   */
  explicit ResizeDialog(QWidget* parent = nullptr);

  ~ResizeDialog() noexcept override;

  /**
   * Returns the chosen width, if there was one.
   *
   * @return the chosen width; nothing if there is none.
   * @since 0.1.0
   */
  [[nodiscard]] Maybe<int> chosen_width() const noexcept
  {
    return m_chosenWidth;
  }

  /**
   * Returns the chosen height, if there was one.
   *
   * @return the chosen height; nothing if there is none.
   * @since 0.1.0
   */
  [[nodiscard]] Maybe<int> chosen_height() const noexcept
  {
    return m_chosenHeight;
  }

 private:
  Ui::ResizeDialogUI* m_ui;
  QIntValidator* m_validator;
  Maybe<int> m_chosenWidth;
  Maybe<int> m_chosenHeight;

  /**
   * Connects a line edit widget to the dialog. This method has no effect if
   * the supplied pointer is null.
   *
   * @param edit a pointer to the line edit widget that will be connected.
   * @since 0.1.0
   */
  void connect_line_edit(QLineEdit* edit) noexcept;

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
  [[nodiscard]] QValidator::State is_valid(const QLineEdit& edit) const
      noexcept;

 private slots:
  /**
   * Checks the validity of the current input state.
   *
   * @since 0.1.0
   */
  void validate_input() noexcept;
};

}  // namespace tactile

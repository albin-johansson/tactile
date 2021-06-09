#pragma once

#include <QDialog>      // QDialog
#include <QLineEdit>    // QLineEdit
#include <QPushButton>  // QPushButton
#include <QValidator>   // QValidator
#include <concepts>     // invocable

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE_UI(ResizeDialog)

namespace tactile {

/**
 * \class ResizeDialog
 *
 * \brief Represents the dialog that is used to select the new size of the
 * current map.
 *
 * \since 0.1.0
 *
 * \headerfile resize_dialog.hpp
 */
class ResizeDialog final : public QDialog
{
  Q_OBJECT

 public:
  /**
   * \param parent a pointer to the parent widget, defaults to null.
   *
   * \since 0.1.0
   */
  explicit ResizeDialog(QWidget* parent = nullptr);

  ~ResizeDialog() noexcept override;

  /**
   * \brief Spawns a resize dialog and blocks the invoking thread.
   *
   * \details This function will invoke the supplied function object if the
   * dialog was closed successfully and a width and height was successfully
   * obtained.
   *
   * \tparam T the type of the function object.
   *
   * \param callback the function object that will be invoked if the dialog
   * was successfully closed.
   *
   * \since 0.1.0
   */
  template <std::invocable<row_t, col_t> T>
  static void Spawn(T&& callback)
  {
    ResizeDialog dialog;
    if (dialog.exec())
    {
      const auto rows = dialog.mChosenHeight;
      const auto cols = dialog.mChosenWidth;
      if (rows && cols)
      {
        callback(*rows, *cols);
      }
    }
  }

 private:
  unique<Ui::ResizeDialog> mUi;
  QIntValidator* mValidator;
  maybe<col_t> mChosenWidth;
  maybe<row_t> mChosenHeight;

  /**
   * \brief Connects a line edit widget to the dialog.
   *
   * \note This method has no effect if the supplied pointer is null.
   *
   * \param edit a pointer to the line edit widget that will be connected.
   *
   * \since 0.1.0
   */
  void ConnectLineEdit(QLineEdit* edit);

  /**
   * \brief Returns a pointer to the "OK" button.
   *
   * \return a pointer to the "OK" button.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto OkButton() const -> QPushButton*;

  /**
   * \brief Indicates whether or not the supplied line edit widget has valid
   * input.
   *
   * \param edit the line edit widget that will be checked.
   *
   * \return `true` if the line edit widget is valid; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto IsValid(const QLineEdit& edit) const -> QValidator::State;

 private slots:
  /**
   * Checks the validity of the current input state.
   *
   * \since 0.1.0
   */
  void ValidateInput();
};

}  // namespace tactile

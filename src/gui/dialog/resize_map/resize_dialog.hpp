#pragma once

#include <QDialog>      // QDialog
#include <QLineEdit>    // QLineEdit
#include <QPushButton>  // QPushButton
#include <QValidator>   // QValidator
#include <concepts>     // invocable

#include "maybe.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"

namespace Ui {
class resize_dialog;
}

namespace tactile::gui {

/**
 * \class resize_dialog
 *
 * \brief Represents the dialog that is used to select the new size of the
 * current map.
 *
 * \since 0.1.0
 *
 * \headerfile resize_dialog.hpp
 */
class resize_dialog final : public QDialog
{
  Q_OBJECT

 public:
  /**
   * \param parent a pointer to the parent widget, defaults to null.
   *
   * \since 0.1.0
   */
  explicit resize_dialog(QWidget* parent = nullptr);

  ~resize_dialog() noexcept override;

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
  static void spawn(T&& callback)
  {
    resize_dialog dialog;
    if (dialog.exec()) {
      const auto rows = dialog.m_chosenHeight;
      const auto cols = dialog.m_chosenWidth;
      if (rows && cols) {
        callback(*rows, *cols);
      }
    }
  }

 private:
  unique<Ui::resize_dialog> m_ui;
  QIntValidator* m_validator;
  maybe<col_t> m_chosenWidth;
  maybe<row_t> m_chosenHeight;

  /**
   * \brief Connects a line edit widget to the dialog.
   *
   * \note This method has no effect if the supplied pointer is null.
   *
   * \param edit a pointer to the line edit widget that will be connected.
   *
   * \since 0.1.0
   */
  void connect_line_edit(QLineEdit* edit);

  /**
   * \brief Returns a pointer to the "OK" button.
   *
   * \return a pointer to the "OK" button.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto ok_button() const -> QPushButton*;

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
  [[nodiscard]] auto is_valid(const QLineEdit& edit) const -> QValidator::State;

 private slots:
  /**
   * Checks the validity of the current input state.
   *
   * \since 0.1.0
   */
  void validate_input();
};

}  // namespace tactile::gui

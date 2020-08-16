#pragma once

#include <QDialog>
#include <QValidator>
#include <optional>

namespace Ui {

class ResizeDialogUI;

}

class QIntValidator;
class QPushButton;
class QLineEdit;

namespace tactile::gui {

/**
 * @class resize_dialog
 *
 * @brief Represents the dialog that is used to select the new size of the
 * current tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile resize_dialog.hpp
 */
class resize_dialog final : public QDialog {
  Q_OBJECT

 public:
  /**
   * @param parent a pointer to the parent widget, defaults to null.
   *
   * @since 0.1.0
   */
  explicit resize_dialog(QWidget* parent = nullptr);

  ~resize_dialog() noexcept override;

  /**
   * @brief Returns the chosen width, if there was one.
   *
   * @return the chosen width; `std::nullopt` if there is none.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto chosen_width() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the chosen height, if there was one.
   *
   * @return the chosen height; `std::nullopt` if there is none.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto chosen_height() const noexcept -> std::optional<int>;

 private:
  Ui::ResizeDialogUI* m_ui;
  QIntValidator* m_validator;
  std::optional<int> m_chosenWidth;
  std::optional<int> m_chosenHeight;

  /**
   * @brief Connects a line edit widget to the dialog.
   *
   * @note This method has no effect if the supplied pointer is null.
   *
   * @param edit a pointer to the line edit widget that will be connected.
   *
   * @since 0.1.0
   */
  void connect_line_edit(QLineEdit* edit) noexcept;

  /**
   * @brief Returns a pointer to the "OK" button.
   *
   * @return a pointer to the "OK" button.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto ok_button() const noexcept -> QPushButton*;

  /**
   * @brief Indicates whether or not the supplied line edit widget has valid
   * input.
   *
   * @param edit the line edit widget that will be checked.
   *
   * @return `true` if the line edit widget is valid; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto is_valid(const QLineEdit& edit) const noexcept
      -> QValidator::State;

 private slots:
  /**
   * Checks the validity of the current input state.
   *
   * @since 0.1.0
   */
  void validate_input() noexcept;
};

}  // namespace tactile::gui

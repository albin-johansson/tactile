#pragma once

#include <QDialog>
#include <QIntValidator>
#include <memory>
#include <optional>

#include "tactile_types.hpp"

class QLineEdit;

namespace Ui {
class TilesetDialogUI;
}

namespace tactile::ui {

class tileset_dialog final : public QDialog {
 public:
  explicit tileset_dialog(QWidget* parent = nullptr);

  ~tileset_dialog() noexcept override;

  /**
   * @brief Returns a shared pointer to the image that was selected.
   *
   * @return a shared pointer to an image; might be null if there is no chosen
   * image.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto chosen_image() const noexcept
      -> const std::shared_ptr<QImage>&;

  /**
   * @brief Returns the chosen tile width.
   *
   * @return the chosen tile width; std::nullopt if no tile width is available.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto chosen_width() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the chosen tile height.
   *
   * @return the chosen tile height; `std::nullopt` if no tile height is
   * available.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto chosen_height() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the name of the selected image.
   *
   * @return the name of the selected image; `std::nullopt` if there is no
   * selected image.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto image_name() const noexcept -> std::optional<QString>;

 private:
  owner<Ui::TilesetDialogUI*> m_ui;
  std::shared_ptr<QImage> m_image;
  std::optional<int> m_width;
  std::optional<int> m_height;
  std::optional<QString> m_imageName;
  std::unique_ptr<QIntValidator> m_validator;
  QPixmap m_defaultImageIcon;

  void validate_input() noexcept;

  [[nodiscard]] auto is_valid() const noexcept -> bool;

  /**
   * Returns a pointer to the "OK" button.
   *
   * @return a pointer to the "OK" button.
   * @since 0.1.0
   */
  [[nodiscard]] auto ok_button() noexcept -> QPushButton*;

  /**
   * Indicates whether or not the supplied line edit widget has valid input.
   *
   * @param edit the line edit widget that will be checked.
   * @return true if the line edit widget is valid; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] auto validate(const QLineEdit& edit) const noexcept
      -> QValidator::State;
};

}  // namespace tactile::ui

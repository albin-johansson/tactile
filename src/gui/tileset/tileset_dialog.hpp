#pragma once

#include <qdialog.h>
#include <qimage.h>
#include <qvalidator.h>

#include <concepts>
#include <optional>

#include "tactile_types.hpp"

class QLineEdit;

namespace Ui {
class tileset_dialog;
}

namespace tactile::gui {

template <typename T>
concept TilesetDialogCallback =
    std::invocable<T, const QImage&, int, int, const QString&>;

class tileset_dialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit tileset_dialog(QWidget* parent = nullptr);

  ~tileset_dialog() noexcept override;

  template <TilesetDialogCallback T>
  static void spawn(T&& callback)
  {
    tileset_dialog dialog;
    if (dialog.exec()) {
      const auto& image = dialog.chosen_image();
      const auto tileWidth = dialog.chosen_width();
      const auto tileHeight = dialog.chosen_height();
      const auto name = dialog.image_name();
      if (!image.isNull() && tileWidth && tileHeight) {
        callback(image, *tileWidth, *tileHeight, name ? *name : "Untitled");
      }
    }
  }

  /**
   * @brief Returns the image that was selected.
   *
   * @return the selected image, which might not represent a valid image.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto chosen_image() const -> const QImage&;

  /**
   * @brief Returns the chosen tile width.
   *
   * @return the chosen tile width; std::nullopt if no tile width is available.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto chosen_width() const -> std::optional<int>;

  /**
   * @brief Returns the chosen tile height.
   *
   * @return the chosen tile height; `std::nullopt` if no tile height is
   * available.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto chosen_height() const -> std::optional<int>;

  /**
   * @brief Returns the name of the selected image.
   *
   * @return the name of the selected image; `std::nullopt` if there is no
   * selected image.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto image_name() const -> std::optional<QString>;

 private:
  owner<Ui::tileset_dialog*> m_ui;
  QImage m_image{};
  std::optional<int> m_width;
  std::optional<int> m_height;
  std::optional<QString> m_imageName;
  QIntValidator* m_validator;
  QPixmap m_defaultImageIcon;

  void validate_input();

  [[nodiscard]] auto is_valid() const -> bool;

  /**
   * Returns a pointer to the "OK" button.
   *
   * @return a pointer to the "OK" button.
   * @since 0.1.0
   */
  [[nodiscard]] auto ok_button() -> QPushButton*;

  /**
   * Indicates whether or not the supplied line edit widget has valid input.
   *
   * @param edit the line edit widget that will be checked.
   * @return true if the line edit widget is valid; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] auto validate(const QLineEdit& edit) const -> QValidator::State;

 private slots:
  void on_imageButton_pressed();

  void on_widthEdit_textChanged();

  void on_heightEdit_textChanged();
};

}  // namespace tactile::gui

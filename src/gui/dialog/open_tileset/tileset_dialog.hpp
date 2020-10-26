#pragma once

#include <QDialog>
#include <QFileInfo>
#include <QImage>
#include <QLineEdit>
#include <QPixmap>
#include <QValidator>
#include <concepts>  // invocable
#include <optional>  // optional

#include "tile_height.hpp"
#include "tile_width.hpp"

namespace Ui {
class tileset_dialog;
}

namespace tactile::gui {

struct tileset_info final
{
  QImage image;
  QString name;
  tile_width tileWidth;
  tile_height tileHeight;
  QFileInfo path;
};

/**
 * \class tileset_dialog
 *
 * \brief Represents the dialog used to add new tilesets.
 *
 * \since 0.1.0
 *
 * \headerfile tileset_dialog.hpp
 */
class tileset_dialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit tileset_dialog(QWidget* parent = nullptr);

  ~tileset_dialog() noexcept override;

  template <std::invocable<const tileset_info&> T>
  static void spawn(T&& callback)
  {
    tileset_dialog dialog;
    if (dialog.exec()) {
      const auto& image = dialog.m_image;
      const auto tileWidth = dialog.m_tileWidth;
      const auto tileHeight = dialog.m_tileHeight;
      if (!image.isNull() && tileWidth && tileHeight) {
        const auto& name = dialog.m_imageName;
        const tileset_info info{
            .image = dialog.m_image,
            .name = name ? *name : QStringLiteral(u"Untitled"),
            .tileWidth = *tileWidth,
            .tileHeight = *tileHeight,
            .path = dialog.m_path};
        callback(info);
      }
    }
  }

 private:
  Ui::tileset_dialog* m_ui{};
  QImage m_image{};
  QString m_path{};
  std::optional<tile_width> m_tileWidth;
  std::optional<tile_height> m_tileHeight;
  std::optional<QString> m_imageName;
  QIntValidator* m_validator;
  QPixmap m_defaultImageIcon;

  void validate_input();

  [[nodiscard]] auto is_valid() const -> bool;

  /**
   * Returns a pointer to the "OK" button.
   *
   * \return a pointer to the "OK" button.
   * \since 0.1.0
   */
  [[nodiscard]] auto ok_button() -> QPushButton*;

  /**
   * Indicates whether or not the supplied line edit widget has valid input.
   *
   * \param edit the line edit widget that will be checked.
   * \return true if the line edit widget is valid; false otherwise.
   * \since 0.1.0
   */
  [[nodiscard]] auto validate(const QLineEdit& edit) const -> QValidator::State;

 private slots:
  void on_imageButton_pressed();

  void on_widthEdit_textChanged();

  void on_heightEdit_textChanged();
};

}  // namespace tactile::gui

#pragma once

#include <QDialog>    // QDialog
#include <QFileInfo>  // QFileInfo

#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE_UI(OpenMapErrorDialog)

namespace tactile {

class OpenMapErrorDialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit OpenMapErrorDialog(QWidget* parent = nullptr);

  ~OpenMapErrorDialog() noexcept override;

  void SetFile(const QFileInfo& path);

  void SetErrorMessage(const QString& message);

 private:
  unique<Ui::OpenMapErrorDialog> mUi;
};

}  // namespace tactile

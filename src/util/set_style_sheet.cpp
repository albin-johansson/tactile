#include "set_style_sheet.hpp"

#include <QFile>

namespace tactile {

void set_style_sheet(QApplication& app, const QString& file)
{
  QFile styleSheet{file};
  if (styleSheet.open(QFile::ReadOnly))
  {
    const QString style{styleSheet.readAll()};
    app.setStyleSheet(style);
  }
  else
  {
    qWarning("Failed to open stylesheet!");
  }
}

}  // namespace tactile

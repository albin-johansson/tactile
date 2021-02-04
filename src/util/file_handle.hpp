#pragma once

#include <QFile>    // QFile
#include <QString>  // QString>

namespace tactile {

class file_handle final
{
 public:
  explicit file_handle(const QString& path);

  ~file_handle();

  auto open(QFile::OpenMode mode) -> bool;

  [[nodiscard]] auto read() -> QByteArray;

  [[nodiscard]] auto file() -> QFile&;

  [[nodiscard]] auto file() const -> const QFile&;

 private:
  QFile m_file;
};

}  // namespace tactile

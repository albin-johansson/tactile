#pragma once

#include <QFile>    // QFile
#include <QString>  // QString>

namespace tactile {

class FileHandle final
{
 public:
  explicit FileHandle(const QString& path);

  ~FileHandle();

  auto Open(QFile::OpenMode mode) -> bool;

  [[nodiscard]] auto Read() -> QByteArray;

  [[nodiscard]] auto File() -> QFile&;

  [[nodiscard]] auto File() const -> const QFile&;

  [[nodiscard]] auto Exists() const -> bool;

 private:
  QFile mFile;
};

}  // namespace tactile
